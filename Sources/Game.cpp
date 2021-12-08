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

Game::Game()
{
	this->initWindow();
	this->initPlayer();
	this->initVariables();
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

void Game::render()
{
	// Clear the screen
	this->window->clear();

	// Render
	this->window->display();
}