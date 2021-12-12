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

void Game::initWorld()
{
	/*
	Set up the background texture
	*/
	if (!this->worldBackgroundTexture.loadFromFile("Textures/background.png"))
		std::cout << "ERROR::GAME::Failed to load background texture \n";
	this->worldBackground.setTexture(this->worldBackgroundTexture);
}

void Game::initPlayer()
{
	/*
	Set up the player
	*/
	this->player = new Player();
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

Game::Game()
{
	this->initWindow();
	this->initWorld();
	this->initPlayer();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window; // Delete window
	delete this->player; // Delete player

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
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::update()
{
	this->updatePollEvents();
	this->player->update();
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

	// Update position of enemies
	unsigned counter = 0;
	for (auto *enemy : this->enemies)
	{
		enemy->update();

		if (enemy->getBounds().top > this->window->getSize().y + 120.f ||
			enemy->getBounds().top < -120.f ||
			enemy->getBounds().left < -120.f ||
			enemy->getBounds().left > this->window->getSize().x + 120.f)
		{
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
			counter = counter - 1;

			// std::cout << "Nombre d'ennemies : " << this->enemies.size() << "\n";
		}

		counter = counter + 1;
	}

	counter = 0;
	for (auto *enemy : this->enemiesRock)
	{
		enemy->update();
		if (enemy->isDestroyed())
		{
			delete this->enemiesRock.at(counter);
			this->enemiesRock.erase(this->enemiesRock.begin() + counter);
			counter = counter - 1;
		}

		counter = counter + 1;
	}
}

void Game::renderWorld()
{
	this->window->draw(this->worldBackground);
}

void Game::render()
{
	// Clear the screen
	this->window->clear();

	// Draw background
	this->renderWorld();

	// Draw Player
	this->player->renderPlayer(*this->window);

	// Draw enemies
	for (auto *enemy : this->enemiesRock)
	{
		enemy->render(this->window);
	}

	for (auto *enemy : this->enemies)
	{
		enemy->render(this->window);
	}

	// Render
	this->window->display();
}