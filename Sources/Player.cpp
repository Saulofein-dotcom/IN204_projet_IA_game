#include "./headers/Player.h"

void Player::initShape()
{
    this->shape.setFillColor(sf::Color::Green);
    this->shape.setSize(sf::Vector2f(100.f, 100.f));
    this->shape.setPosition(sf::Vector2f(100.f, 100.f));
}

void Player::initVariables()
{
    this->health = 100.f; // vie du joueur
    this->speed = 10.f;   // nombre de pixels parcoururs par appui sur une touche
}

Player::Player()
{
    this->initVariables();
    this->initShape();
}

Player::~Player()
{
}

void Player::renderPlayer()
{
}

void Player::updatePosition()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
        this->y -= this->speed; // Use move sur shape rather
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        this->x -= this->speed;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        this->y += this->speed;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        this->x += this->speed;
    // Change position
}