#include "headers/Game.h"

void Game::initWindow()
{
	/*
	Set up the window, background, title, settings of the screen
	*/
	this->window = new RenderWindow(VideoMode(1300, 900), "Link Revenge", Style::Close | Style::Titlebar);
	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);
}

void Game::initPlayer()
{
	this->player = new Player();
}

void Game::initVariables()
{
	this->player->initVariables();
}

void Game::initEnemies()
{
	this->spawnTimerMax = 50.f;
	this->spawnTimer = this->spawnTimerMax;
}

Game::Game()
{
	this->initWindow();
	this->initPlayer();
	this->initVariables();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
	delete this->player;
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::update()
{
	this->updatePollEvents();
	this->player->updatePosition();
	this->updateEnemies();
}

void Game::updatePollEvents()
{
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
	//Spawning enemies
	this->spawnTimer += 0.5f;
	if(this->spawnTimer > spawnTimerMax)
	{
		bool randomLateral = rand() % 2; // If randomLateral == 0, the enemy spawn on the left of the screen, otherwise on the right
		bool randomVertical = rand() % 2; // If randomLateral == 0, the enemy spawn on the top of the screen, otherwise on the bottom
		if(randomLateral==0 && randomVertical==0)
		{

		}else if(randomLateral==0 && randomVertical==1)
		{

		}else if(randomLateral==1 && randomVertical==0)
		{

		}else
		{

		}
		this->enemies.push_back(new Enemy(10, 10, 10, 10));
		this->spawnTimer = 0.f;
	}

	//Update position of enemies
	for (auto *enemy : this->enemies)
	{
		enemy->update();
	}

}

void Game::render()
{
	// Clear the screen
	this->window->clear();
	
	//Draw enemies
	for(auto *enemy : this->enemies)
	{
		enemy->render(this->window);
	}

	// Render
	this->window->display();
}