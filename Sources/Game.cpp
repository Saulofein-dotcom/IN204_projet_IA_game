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
	if (!this->worldBackgroundTexture.loadFromFile("../../Textures/background.png"))
		std::cout << "ERROR::GAME::Failed to load background texture \n";
	this->worldBackground.setTexture(this->worldBackgroundTexture);
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
	if (!this->font.loadFromFile("Fonts/ARCADE.TTF"))
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
	this->initGUI();
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
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::update()
{
	this->updateClock();
	this->updateGUI();
	this->updatePollEvents();
	this->updatePlayer();
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

void Game::updatePlayer()
{
	this->player->update(); // Move player, sword, udpdate fireballs

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
	this->window->close();
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
	this->renderGUI();

	// Render
	this->window->display();
}