#include "headers/Game.h"
#include<omp.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include"ppo_torch.cpp"
#include "./create_plot/pbPlots.hpp"
#include "./create_plot/supportLib.hpp"

using namespace std;

template<typename A>
double average(std::vector<A> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<double>(v.size());
    return std::reduce(v.begin(), v.end()) / count;
}

auto Game::step(int action, int stackFrame)
{
	vector<double> state(stackFrame*100*100);
	double reward;
	int a = action;
	for(int i = 0; i < stackFrame; i++)
	{
		
		this->update(a);
		this->render();

		Image imageCapture = this->saveImage();
		std::vector<unsigned> compressedImage = imageToVectorC(100, 100, imageCapture);
		copy(compressedImage.begin(), compressedImage.end(), state.begin() + i*compressedImage.size());
	}
	this->end ? reward = -100.0 : reward = 1.0;
	return make_tuple(T::tensor(state).unsqueeze(0),reward , this->end);
}

void Game::initWindow()
{
	/*
	Set up the window, background, title, settings of the screen
	*/
	this->window = new RenderWindow(VideoMode(1300, 900), "Link Revenge", Style::Close | Style::Titlebar);
	this->window->setFramerateLimit(60);
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
	this->spawnTimerMax = 25.f;
	this->spawnTimer = this->spawnTimerMax;

	this->spawnTimerMaxRock = 50.f;
	this->spawnTimerRock = this->spawnTimerMaxRock;
}

void Game::initGUI()
{
	// Load font
	if (!this->font.loadFromFile("../../Fonts/ARCADE.TTF"))
		std::cout << "ERROR::GAME::Failed to load font\n";

	// Init time text
	this->timeText.setFont(this->font);
	this->timeText.setCharacterSize(75);
	this->timeText.setFillColor(sf::Color::White);
	this->timeText.setString("00:00");
	this->timeText.setPosition(30.f, 10.f);

	// Init gameOverText
	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(60);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("Game Over");
	this->gameOverText.setPosition(
		this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f);
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
	for (auto *i : this->enemiesRock)
	{
		delete i;
	}
}

void Game::run()
{
	int frame = 0;
	int stackNumber = 4;
	vector<double> state(stackNumber*100*100);

	random_device rd;  // Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    uniform_real_distribution<> dis(-0.05, 0.05);
	int N = 20;
    int batch_size = 64;
    int n_epochs = 10;
    double alpha = 0.0003;
    int action_space_n = 4;
    int observation_space_shape = stackNumber * 100 * 100;
    double gamma = 0.99;
    double gae_lambda = 0.95;
    double policy_clip = 0.2;

	RGBABitmapImageReference *imageRef = CreateRGBABitmapImageReference();

	Agent agent = Agent(action_space_n, observation_space_shape, gamma, alpha, gae_lambda, policy_clip, batch_size, n_epochs);
	int n_games = 10000;

	vector<long> score_history;
    
    double best_score = std::numeric_limits<double>::min();
    long learn_iters = 0;
    double avg_score = 0;
    long n_steps = 0;

	for(int i = 0; i < n_games ; i++)
    {
		int time = 0;
		
		
        bool done = false;
		this->end = false;
		frame = 0;

		while(frame < stackNumber)
		{
			this->update(5);
			this->render();

			Image imageCapture = this->saveImage();
			std::vector<unsigned> compressedImage = imageToVectorC(100, 100, imageCapture);
			copy(compressedImage.begin(), compressedImage.end(), state.begin() + frame*compressedImage.size());
			
			frame++;
		}
		frame = 0;
        T::Tensor observation = T::tensor(state).unsqueeze(0);

        long score = 0;
        while(!done)
        {
			
            c10::Scalar action ,prob, val;
            tie(action, prob, val) = agent.choose_action(observation);

            double reward;
            T::Tensor observation_;
			tie(observation_, reward, done) = step(action.to<int>(), stackNumber);
            n_steps += 1;
            score += reward;
            
            agent.remember(observation, action, prob, val, reward, done);
            if(n_steps % N == 0)
            {
                agent.learn();
                learn_iters += 1;
            }
			time ++;
            observation = observation_;
			if(time > 600) 
			{
				done = true;
				score += 100;
			}


        }
        score_history.push_back(score);
        int minValue;
        score_history.size() < 100 ? minValue = score_history.size() : minValue = 100;
        vector<int64_t> score_tmp(score_history.end() - minValue, score_history.end());
        
        double avg_score = average(score_tmp);
        if(avg_score > best_score)
        {
            best_score = avg_score;
            agent.save_models();
        }

        cout << "episode " << i << " score "<< score << ", avg_score : " << avg_score
                << ", time steps " << n_steps << ", learning_steps " << learn_iters << endl;

        /*================ PLOT SCORE =================*/

        vector<double> x(score_history.size());
        for(int l = 0; l < score_history.size(); l++)
        {
            x[l] = l+1;
        }
        
        vector<double> running_avg(score_history.size());
        for(int m = 0; m < running_avg.size(); m++)
        {
            int tmpMax;
            m - 100 > 0 ? tmpMax = m-100 : tmpMax = 0;

            vector<int64_t> score_tmp_run(score_history.begin() + tmpMax, score_history.begin() + m + 1);
            running_avg[m] = average(score_tmp_run);
            
        }

        StringReference* error = new StringReference();
        DrawScatterPlot(imageRef, 600, 400, &x, &running_avg, error);

        vector<double> *pngData = ConvertToPNG(imageRef->image);

        WriteToFile(pngData, "../create_plot/plotScore.png");
        DeleteImage(imageRef->image);
        delete error;
        
    }

	
	
}

void Game::update(int a)
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

void Game::updatePlayer(int a)
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
			posX = -100.f;
			posY = static_cast<float>(rand() % this->window->getSize().y);
		}
		else if (randomWindow == 1) // Top
		{
			posX = static_cast<float>(rand() % this->window->getSize().x);
			posY = -100.f;
		}
		else if (randomWindow == 2) // Right
		{
			posX = this->window->getSize().x + 100.f;
			posY = static_cast<float>(rand() % this->window->getSize().y);
		}
		else // Bottom
		{
			posX = static_cast<float>(rand() % this->window->getSize().x);
			posY = this->window->getSize().y + 100.f;
		}
		float posXCenter = (float)this->window->getSize().x / 4 + (rand() % this->window->getSize().x / 2);
		float posYCenter = (float)this->window->getSize().y / 3 + (rand() % this->window->getSize().y / 3);

		this->enemies.push_back(new Enemy(posX, posY, posXCenter, posYCenter));
		this->spawnTimer = 0.f;
	}

	if (this->spawnTimerRock > this->spawnTimerMaxRock)
	{
		int pos_x = (rand() % (this->window->getSize().x - 150));
		int pos_y_explode = (rand() % (this->window->getSize().y - 150));
		int pos_y = pos_y_explode - this->window->getSize().y;
		this->enemiesRock.push_back(new EnemyRock(pos_x, pos_y, pos_x, pos_y_explode));
		this->spawnTimerRock = 0.f;
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

		// Enemies hit by a fireball are destroyed and the fireball is deleted
		for (int j = 0; j < this->player->fireballs.size() && !enemy_removed; ++j)
		{
			if (this->player->fireballs[j]->getBounds().intersects(this->enemies[i]->getBounds()))
			{
				delete this->player->getFireballs().at(j);
				this->player->fireballs.erase(this->player->fireballs.begin() + j);
				delete this->enemies.at(i);
				this->enemies.erase(this->enemies.begin() + i);
				enemy_removed = true;
			}
		}

		// Enemies out of bounds are deleted
		if (!enemy_removed)
		{
			if (this->enemies[i]->getBounds().top > this->window->getSize().y + 120.f ||
				this->enemies[i]->getBounds().top < -120.f ||
				this->enemies[i]->getBounds().left < -120.f ||
				this->enemies[i]->getBounds().left > this->window->getSize().x + 120.f)
			{
				delete this->enemies.at(i);
				this->enemies.erase(this->enemies.begin() + i);
			};
		}
		enemy_removed = false;
	}

	int counter = 0;
	for (auto *enemy : this->enemiesRock)
	{
		enemy->update(); // Move the rock enemy

		// Check for collision with player
		if (this->player->getBounds().intersects(enemy->getBounds()) && enemy->isDangerous())
		{
			// End of game
			this->triggerEndOfGame();
		}
		if (enemy->isDestroyed())
		{
			delete this->enemiesRock.at(counter);
			this->enemiesRock.erase(this->enemiesRock.begin() + counter);
			--counter;
		}

		++counter;
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

Image Game::saveImage()
{
	sf::Texture texture;
	texture.create(this->window->getSize().x, this->window->getSize().y);
	texture.update(*this->window);
	return texture.copyToImage();
}

std::vector<unsigned> Game::imageToVectorC(unsigned width, unsigned height, Image myImage)
{
	std::vector<unsigned> myVectorImage(width * height);

	Image test;
	test.create(width, height); 

	Vector2u sizeOriginal = myImage.getSize();
	unsigned x_step = sizeOriginal.x / width;
	unsigned y_step = sizeOriginal.y / width;

	#pragma omp parallel for
	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			unsigned red = myImage.getPixel(x_step*i, y_step*j).r;
			unsigned green = myImage.getPixel(x_step*i, y_step*j).g;
			unsigned blue = myImage.getPixel(x_step*i, y_step*j).b;
			unsigned gray = (red + green + blue) / 3;

			myVectorImage[i * width + j] = gray;
			test.setPixel(i, j, Color(gray, gray, gray));
		}
	}
	test.saveToFile("testImage.png");

	return myVectorImage;

}