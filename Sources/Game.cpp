#include "headers/Game.h"

void Game::initWindow()
{
    this->window = new RenderWindow(VideoMode(1280, 720), "Link Revenge", Style::Close | Style::Titlebar);
	this->window->setFramerateLimit(60);
	this->window->setVerticalSyncEnabled(false);
}

Game::Game()
{
    this->initWindow();
}

Game::~Game()
{
    delete this->window;
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

void Game::render()
{
    this->window->clear();

    this->window->display();
}