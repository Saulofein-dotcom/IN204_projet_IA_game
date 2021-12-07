#include "Game.h"

void Game::initWindow()
{
	this->window = new RenderWindow(VideoMode(800, 600), "Tuto_Game", Style::Close | Style::Titlebar);
	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);
}

void Game::initTextures()
{
	this->textures["BULLET"] = new Texture();
	this->textures["BULLET"]->loadFromFile("Textures/bullet.png");
}

void Game::initGUI()
{
	//Load fonts
	if (!this->font.loadFromFile("Fonts/ARCADECLASSIC.ttf"))
		std::cout << "ERROR::GAME::Failed to load font \n";

	//Initialize text
	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(24);
	this->pointText.setFillColor(Color::White);
	this->pointText.setString("My game");

	this->playerHpBar.setSize(Vector2f(300.f, 25.f));
	this->playerHpBar.setFillColor(Color::Green);
	this->playerHpBar.setPosition(Vector2f(20.f, 40.f));

	this->playerHpBarBack = this->playerHpBar;
	this->playerHpBarBack.setFillColor(Color(200, 0, 0, 200));
	this->playerHpBarBack.setOutlineColor(Color::White);
	this->playerHpBarBack.setOutlineThickness(3.f);
}

void Game::initWorld()
{
	if (!this->worldBackgroundTexture.loadFromFile("Textures/background1.jpg"))
		std::cout << "ERROR::GAME::Failed to load background texture \n";
	this->worldBackground.setTexture(this->worldBackgroundTexture);
}

void Game::initSystem()
{
	this->points = 0;
}

void Game::initPlayer()
{
	this->player = new Player();
}

void Game::initEnemies()
{
	this->spawnTimerMax = 50.f;
	this->spawnTimer = this->spawnTimerMax;
}

Game::Game()
{
	this->initWindow();
	this->initTextures();
	this->initGUI();
	this->initWorld();
	this->initSystem();
	this->initPlayer();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
	delete this->player;

	//Delete textures
	for (auto &i : this->textures)
	{
		delete i.second;
	}

	//Delete bullets
	for (auto *i : this->bullets)
	{
		delete i;
	}

	//Delete enemies
	for (auto *i : this->enemies)
	{
		delete i;
	}
}

//Functions
void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();	
		this->render();	
	}
}

void Game::updatePollEvents()
{
	Event e;
	while (this->window->pollEvent(e))
	{
		if (e.type == Event::Closed)
			this->window->close();
		if (e.KeyPressed && e.key.code == Keyboard::Escape)
			this->window->close();
	}
}

void Game::updateInput()
{
	//Move player
	if (Keyboard::isKeyPressed(Keyboard::Q))
		this->player->move(-1.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::S))
		this->player->move(0.f, 1.f);
	if (Keyboard::isKeyPressed(Keyboard::D))
		this->player->move(1.f, 0.f);
	if (Keyboard::isKeyPressed(Keyboard::Z))
		this->player->move(0.f, -1.f);

	if (Keyboard::isKeyPressed(Keyboard::Space) && this->player->canAttack())
	{
		this->bullets.push_back(
			new Bullet(
				this->textures["BULLET"], 
				this->player->getPos().x + this->player->getBounds().width/2.f - 5.f, 
				this->player->getPos().y,
				0.f,
				-1.f,
				10.f)
		);
	}
}

void Game::updateBullet()
{
	unsigned counter = 0;
	for (auto *bullet : this->bullets)
	{
		bullet->update();

		//Delete bullet when it reaches the top of the screen
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			delete this->bullets.at(counter);
			this->bullets.erase(this->bullets.begin() + counter);
			--counter;
		}

		++counter;
	}
}

void Game::updateGUI()
{
	std::stringstream ss;

	ss << "Points  " << this->points;

	this->pointText.setString(ss.str());

	//Update player GUI
	float hpPercent = static_cast<float>((float) this->player->getHp() / this->player->getHpMax());
	this->playerHpBar.setSize(Vector2f(this->playerHpBarBack.getSize().x * hpPercent, this->playerHpBar.getSize().y));
}

void Game::updateWorld()
{
}

void Game::updateCollision()
{
	//Left size
	if (this->player->getBounds().left <= 0.f)
	{
		this->player->setPosition(0.f, this->player->getBounds().top);
	}

	//Right size
	else if (this->player->getBounds().left + this->player->getBounds().width >= this->window->getSize().x)
	{
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	}

	if (this->player->getBounds().top <= 0.f)
	{
		this->player->setPosition(this->player->getBounds().left, 0.f);
	}

	else if (this->player->getBounds().top + this->player->getBounds().height >= this->window->getSize().y)
	{
		this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
	}
}

void Game::updateEnemies()
{
	//Spawning
	this->spawnTimer += 0.5f;
	if(this->spawnTimer > spawnTimerMax)
	{
		this->enemies.push_back(new Enemy(rand() % this->window->getSize().x-20.f, -100.f));
		this->spawnTimer = 0.f;
	}

	//Update
	unsigned counter = 0;
	for (auto *enemy : this->enemies)
	{
		enemy->update();

		//Delete enemy when it reaches the bottom of the screen
		if (enemy->getBounds().top > this->window->getSize().y)
		{
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
			--counter;
		}

		else if (enemy->getBounds().intersects(this->player->getBounds()))
		{
			this->player->loseHp(enemy->getDamage());
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
			--counter;
		}

		++counter;
	}


}

void Game::updateCombat()
{
	for (size_t i = 0; i < this->enemies.size(); ++i)
	{
		bool deleted_enemy = false;
		for (size_t k = 0; k < this->bullets.size() && !deleted_enemy; ++k)
		{
			if (this->enemies[i]->getBounds().intersects(this->bullets[k]->getBounds()))
			{
				this->points += this->enemies[i]->getPoints();

				delete this->enemies[i];
				this->enemies.erase(this->enemies.begin() + i);

				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);

				deleted_enemy = true;

			}
		}

	}
}

void Game::update()
{
	this->updatePollEvents();
	this->updateInput();
	this->player->update();
	this->updateCollision();
	this->updateBullet();
	this->updateEnemies();
	this->updateCombat();
	this->updateGUI();
	this->updateWorld();
}

void Game::renderWorld()
{
	this->window->draw(this->worldBackground);
}

void Game::renderGUI()
{
	this->window->draw(this->pointText);
	this->window->draw(this->playerHpBarBack);
	this->window->draw(this->playerHpBar);
}

void Game::render()
{
	this->window->clear();

	//Draw background
	this->renderWorld();

	//Draw all the stuffs
	this->player->render(*this->window);

	for (auto *bullet : this->bullets)
	{
		bullet->render(this->window);
	}

	for (auto *enemy : this->enemies)
	{
		enemy->render(this->window);
	}

	this->renderGUI();

	this->window->display();
}

