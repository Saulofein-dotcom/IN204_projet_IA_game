#include "headers/Game.h"
#include<omp.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <limits.h>
#include <chrono>
#include <thread>

#include<torch/torch.h>
#include"ppo_torch.cpp"
#include "./create_plot/pbPlots.hpp"
#include "./create_plot/supportLib.hpp"
#include "ProximalPolicyOptimization.h"
#include "Models.h"

enum STATUS {
    PLAYING,
    WON,
    LOST,
    RESETTING
};

using namespace std;


template<typename A>
double average(std::vector<A> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<double>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}


auto Game::step(int action, long n_in, long timestep, T::Tensor& actualState, int width, int height, int frames)
{
	this->update(action);
	this->render();

	torch::Tensor state = torch::zeros({1, n_in}, torch::kF64);
	std::memcpy(state.data_ptr(), actualState.data_ptr() + width*height*sizeof(double), width*height*(frames-1) * sizeof(double));

	Image imageCapture = this->saveImage();
	T::Tensor compressedImage = imageToVectorC(width, height, imageCapture);
	std::memcpy(state.data_ptr() + width*height*(frames-1)*sizeof(double), compressedImage.data_ptr(), width*height*sizeof(double));
	

	torch::Tensor done = torch::zeros({1, 1}, torch::kF64);
    STATUS status;
	actualState = state;

	if(timestep > 150000)
	{
		status = WON;
		done[0][0] = 1.0;
	}else if(this->end)
	{
		status = LOST;
		done[0][0] = 0.0;
		this->end = false;
	}else{
		status=PLAYING;
		done[0][0] = 0.0;
	}

	return std::make_tuple(actualState, status, done);

}

auto Reward(int status) -> torch::Tensor
{
	torch::Tensor reward = torch::full({1, 1}, 0, torch::kF64);

	switch (status)
	{
		case PLAYING:
			reward[0][0] += 1.;
			break;
		case WON:
			reward[0][0] += 100.;
			printf("won, reward: %f\n", reward[0][0].item<double>());
			break;
		case LOST:
			reward[0][0] -= 100.;
			//printf("lost, reward: %f\n", reward[0][0].item<double>());
			break;
	}

	return reward;
}

void Game::initWindow()
{
	/*
	Set up the window, background, title, settings of the screen
	*/
	this->window = new RenderWindow(VideoMode(200, 200), "Link Revenge", Style::Close | Style::Titlebar);
	this->window->setFramerateLimit(120);
	this->window->setVerticalSyncEnabled(false);
}

void Game::initWorld()
{
	/*
	Set up the background texture
	*/
	this->end = false;
	/*
	if (!this->worldBackgroundTexture.loadFromFile("../../Textures/background.png"))
		std::cout << "ERROR::GAME::Failed to load background texture \n";
	this->worldBackground.setTexture(this->worldBackgroundTexture);
	*/
}

void Game::initPlayer()
{
	/*
	Set up the player
	*/
	this->player = new Player(this->window);
}

void Game::initEnemies()
{
	/*
	Initialize variables of the enemies
	*/
	this->spawnTimerMax = 7.f;
	this->spawnTimer = this->spawnTimerMax;

}


void Game::initClock()
{
	this->clock.restart();
}

Game::Game()
{
	this->initWindow();
	this->initWorld();
	//this->initGUI();
	this->initPlayer();
	this->initClock();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window; // Delete window
	delete this->player; // Delete player and fireballs

	// Delete enemies
	for (auto *i : this->enemies)
	{
		delete i;
	}

}

void Game::runTest()
{
	// Model.
	int width = 100;
	int height = 100;
	int frames = 4;
    long n_in = width * height * frames;
    uint n_out = 4;
    double std = 1e-2;

	ActorCritic ac(n_in, n_out, std);
    ac->to(torch::kF64);
    ac->normal(0., std);
	ac->eval();
    torch::load(ac, "../tmp/ppo/best_model.pt");

	VT states;
	VT actions;

	uint n_iter =10000;

	// Counter.
    uint c = 0;
	uint32_t timestep = 0;

	torch::Tensor state = torch::zeros({1, 0}, torch::kF64);

	for(int i = 0; i < frames; i++)
	{
		this->update(T::zeros(n_out, torch::kF64));
		this->render();
		Image imageCapture = this->saveImage();
		T::Tensor compressedImage = imageToVectorC(width, height, imageCapture);
		state = T::cat({state, compressedImage}, 1);
	}

	for (uint i=0;i<n_iter;i++)
    {
		auto av = ac->forward(state);
        auto action = std::get<0>(av);

		uint actionChoose = 0;
		double max_value = action[0][0].item<double>();
		for(int i = 1; i < n_out; i++)
		{
			if(max_value < action[0][i].item<double>()) 
			{
				actionChoose = i;
				max_value = action[0][i].item<double>();
			}
		}

        auto sd = this->step(actionChoose, n_in, timestep, state, width, height, frames);

        // Check for done state.
        auto done = std::get<2>(sd);

        if (done[0][0].item<double>() == 1.) 
        {
            this->resetGame();
        }
    }

}

void Game::run()
{
	// Model.
	int width = 100;
	int height = 100;
	int frames = 8;
    long n_in = width * height * frames;
    uint n_out = 4;
    double std = 1e-2;

	ActorCritic ac(n_in, n_out, std);
    ac->to(torch::kF64);
    ac->normal(0., std);
    torch::optim::Adam opt(ac->parameters(), 1e-3);

	// Training loop.
    uint n_iter =10000;
    uint n_steps = 2048;
    uint n_epochs = 5000;
    uint mini_batch_size = 512;
    uint ppo_epochs = 4;
    double beta = 1e-3;

    VT states;
    VT actions;
    VT rewards;
    VT dones;

    VT log_probs;
    VT returns;
    VT values;
	
	// Counter.
    uint c = 0;
	uint timestep = 0;
	vector<double> score_history;
	torch::Tensor state = torch::zeros({1, 0}, torch::kF64);

	RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();

	// Average reward.
    double best_avg_reward = INT32_MIN;
    double avg_reward = 0.;

	for(int i = 0; i < frames; i++)
	{
		this->update(T::zeros(n_out, torch::kF64));
		this->render();
		Image imageCapture = this->saveImage();
		T::Tensor compressedImage = imageToVectorC(width, height, imageCapture);
		state = T::cat({state, compressedImage}, 1);
	}

	for (uint e=1;e<=n_epochs;e++)
    {
		timestep = 0;
		printf("epoch %u/%u\n", e, n_epochs);
		for (uint i=0;i<n_iter;i++)
        {
			states.push_back(state);
			auto av = ac->forward(states[c]);

            actions.push_back(std::get<0>(av));
            values.push_back(std::get<1>(av));
            log_probs.push_back(ac->log_prob(actions[c]));
			
			
			uint action = 0;
			double max_value = actions[c][0][0].item<double>();
			for(int i = 1; i < n_out; i++)
			{
				if(max_value < actions[c][0][i].item<double>()) 
				{
					action = i;
					max_value = actions[c][0][i].item<double>();
				}
			}
			

			auto sd = this->step(action, n_in, timestep, state, width, height, frames);

			// New state.
            rewards.push_back(Reward(std::get<1>(sd)));
            dones.push_back(std::get<2>(sd));

			avg_reward += rewards[c][0][0].item<double>()/n_iter;

            // episode, agent_x, agent_y, goal_x, goal_y, AGENT=(PLAYING, WON, LOST, RESETTING)
            

            if (dones[c][0][0].item<double>() == 1.) 
            {
				this->resetGame();
			}
			c++;

			if (c%n_steps == 0)
            {
                printf("Updating the network.\n");
                values.push_back(std::get<1>(ac->forward(states[c-1])));

                returns = PPO::returns(rewards, dones, values, .99, .9);

                torch::Tensor t_log_probs = torch::cat(log_probs).detach();
                torch::Tensor t_returns = torch::cat(returns).detach();
                torch::Tensor t_values = torch::cat(values).detach();
                torch::Tensor t_states = torch::cat(states);
                torch::Tensor t_actions = torch::cat(actions);
                torch::Tensor t_advantages = t_returns - t_values.slice(0, 0, n_steps);

                PPO::update(ac, t_states, t_actions, t_log_probs, t_returns, t_advantages, opt, n_steps, ppo_epochs, mini_batch_size, beta);
            
                c = 0;

                states.clear();
                actions.clear();
                rewards.clear();
                dones.clear();

                log_probs.clear();
                returns.clear();
                values.clear();
            }
			timestep++;
		}
		cout << "avg_reward : " << avg_reward <<endl;
		cout << timestep << endl;
		// Save the best net.
        if (avg_reward > best_avg_reward) {

            best_avg_reward = avg_reward;
            printf("Best average reward: %f\n", best_avg_reward);
            torch::save(ac, "../tmp/ppo/best_model.pt");
        }

		/*================ PLOT SCORE =================*/
		score_history.push_back(avg_reward);
		vector<double> x(score_history.size());
		for(long l = 0; l < score_history.size(); l++)
		{
			x[l] = l+1;
		}

		StringReference* error = new StringReference();
		DrawScatterPlot(imageRef, 600, 400, &x, &score_history, error);

		vector<double> *pngData = ConvertToPNG(imageRef->image);

		WriteToFile(pngData, "../create_plot/plotScore.png");
		DeleteImage(imageRef->image);
		delete error;

        avg_reward = 0.;

		this->resetGame();
	}
}

	
	


void Game::update(T::Tensor a)
{
	this->updateClock();
	//this->updateGUI();
	this->updatePollEvents();
	this->updatePlayer(a);
	this->updateEnemies();
}

void Game::update(uint a)
{
	this->updateClock();
	//this->updateGUI();
	this->updatePollEvents();
	this->updatePlayer(a);
	this->updateEnemies();
}


void Game::updateClock()
{
	this->elapsedTime = this->clock.getElapsedTime();
}

void Game::updateGUI()
{
	std::stringstream ss;
	ss << std::setw(2)
	   << std::setfill('0')
	   << static_cast<int>(this->elapsedTime.asSeconds()) / 60
	   << ":"
	   << std::setw(2)
	   << std::setfill('0')
	   << static_cast<int>(this->elapsedTime.asSeconds()) % 60;

	this->timeText.setString(ss.str());
}

void Game::updatePlayer(T::Tensor a)
{
	this->player->update(a[0].item<double>(), a[1].item<double>(), a[2].item<double>(), a[3].item<double>()); // Move player, sword, udpdate fireballs

	// TODO : refactoring + update sword collision according to player collision
	if (this->player->getBounds().left < 0)
		this->player->setPosition(0.f, this->player->getBounds().top);
	else if (this->player->getBounds().left + this->player->getBounds().width > this->window->getSize().x)
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	if (this->player->getBounds().top + this->player->getBounds().height > this->window->getSize().y)
		this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
	else if (this->player->getBounds().top < 0)
		this->player->setPosition(this->player->getBounds().left, 0.f);
}

void Game::updatePlayer(uint a)
{
	this->player->update(a); // Move player, sword, udpdate fireballs

	// TODO : refactoring + update sword collision according to player collision
	if (this->player->getBounds().left < 0)
		this->player->setPosition(0.f, this->player->getBounds().top);
	else if (this->player->getBounds().left + this->player->getBounds().width > this->window->getSize().x)
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	if (this->player->getBounds().top + this->player->getBounds().height > this->window->getSize().y)
		this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
	else if (this->player->getBounds().top < 0)
		this->player->setPosition(this->player->getBounds().left, 0.f);
}

void Game::updatePollEvents()
{
	/*
		Get events to close the window
	*/
	Event e;
	while (this->window->pollEvent(e))
	{
		if (e.type == Event::Closed)
			this->window->close();
		else if (e.KeyPressed && e.key.code == Keyboard::Escape)
			this->window->close();
	}
}

void Game::updateEnemies()
{
	// Spawning enemies
	this->spawnTimer += 0.5f;
	this->spawnTimerRock += 0.5f;

	if (this->spawnTimer > spawnTimerMax)
	{
		int randomWindow = rand() % 4; // If randomLateral == 0, the enemy spawn on the left of the screen, otherwise on the right
		float posX = 10.f;
		float posY = 10.f;
		if (randomWindow == 0) // Left
		{
			posX = -20.f;
			posY = static_cast<float>(rand() % this->window->getSize().y);
		}
		else if (randomWindow == 1) // Top
		{
			posX = static_cast<float>(rand() % this->window->getSize().x);
			posY = -20.f;
		}
		else if (randomWindow == 2) // Right
		{
			posX = this->window->getSize().x + 20.f;
			posY = static_cast<float>(rand() % this->window->getSize().y);
		}
		else // Bottom
		{
			posX = static_cast<float>(rand() % this->window->getSize().x);
			posY = this->window->getSize().y + 20.f;
		}
		float posXCenter = (float)this->window->getSize().x / 4 + (rand() % this->window->getSize().x / 2);
		float posYCenter = (float)this->window->getSize().y / 3 + (rand() % this->window->getSize().y / 3);

		this->enemies.push_back(new Enemy(posX, posY, posXCenter, posYCenter));
		this->spawnTimer = 0.f;
	}

	

	// Update position of skeleton enemies
	bool enemy_removed = false;
	for (int i = 0; i < this->enemies.size(); ++i)
	{
		// Move the enemy
		this->enemies[i]->update();

		// Check for collision with player
		if (this->enemies[i]->getBounds().intersects(this->player->getBounds()))
		{
			// End of game
			this->triggerEndOfGame();
		}

		

		// Enemies out of bounds are deleted
		if (!enemy_removed)
		{
			if (this->enemies[i]->getBounds().top > this->window->getSize().y + 20.f ||
				this->enemies[i]->getBounds().top < -20.f ||
				this->enemies[i]->getBounds().left < -20.f ||
				this->enemies[i]->getBounds().left > this->window->getSize().x + 20.f)
			{
				delete this->enemies.at(i);
				this->enemies.erase(this->enemies.begin() + i);
			};
		}
		enemy_removed = false;
	}

	
}

void Game::triggerEndOfGame()
{
	/*
		This function triggers the end of the game which for now is just
		the window closing
	*/
	//this->window->close();
	this->end = true;
}

// Accessors
std::vector<Enemy *> Game::getEnemies()
{
	return this->enemies;
}

void Game::renderWorld()
{
	this->window->draw(this->worldBackground);
}

void Game::renderGUI()
{
	this->window->draw(this->timeText);
}

void Game::render()
{
	// Clear the screen
	this->window->clear();

	// Draw background
	this->renderWorld();

	// Draw Player
	this->player->renderPlayer(*this->window);

	// Draw Fireballs
	this->player->renderFireballs(*this->window);

	// Draw enemies
	for (auto *enemy : this->enemiesRock)
	{
		enemy->render(this->window);
	}

	for (auto *enemy : this->enemies)
	{
		enemy->render(this->window);
	}

	// Draw the GUI
	//this->renderGUI();

	// Render
	this->window->display();
}

void Game::resetGame()
{

	for(int k = 0; k < this->enemies.size(); k++)
	{
		
		delete this->enemies.at(k);
		
	}
	this->end = false;
	this->enemies.clear();
	this->player->initPosition();
}

Image Game::saveImage()
{
	sf::Texture texture;
	texture.create(this->window->getSize().x, this->window->getSize().y);
	texture.update(*this->window);
	return texture.copyToImage();
}

torch::Tensor Game::imageToVectorC(unsigned width, unsigned height, Image myImage)
{
	torch::Tensor myVectorImage = T::zeros({1, width*height}, torch::kF64);
	//std::vector<unsigned> myVectorImageColored(width * height*3);


	//Image test;
	//test.create(width, height); 

	Vector2u sizeOriginal = myImage.getSize();
	long x_step = sizeOriginal.x / width;
	long y_step = sizeOriginal.y / width;

	#pragma omp parallel for schedule(dynamic, 10)
	for(long j = 0; j < height; j++)
	{
		for(long i = 0; i < width; i++)
		{
			auto pixels = myImage.getPixel(x_step*i, y_step*j);
			unsigned red = pixels.r;
			unsigned green = pixels.g;
			unsigned blue = pixels.b;
			double gray = 0.299 * red + 0.587 * green + 0.114 * blue;

			//myVectorImageColored[3 * j * width + 3 * i] = red;
			//myVectorImageColored[3 * j * width + 3 * i + 1] = green;
			//myVectorImageColored[3 * j * width + 3 * i + 2] = blue;

			myVectorImage[0][j * width + i] = (double)gray/255;
			//test.setPixel(i, j, Color(gray, gray, gray));
		}
	}
	//test.saveToFile("../testImage.png");

	return myVectorImage;

}

