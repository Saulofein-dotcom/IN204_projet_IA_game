#include "./headers/Player.h"

Player::Player()
{
    Player::initVariables();
}

void Player::initVariables()
{
    this->health = 100;
    this->speed = 10; // nombre de pixels parcoururs par appui sur une touche
}
void Player::move(sf::Event ev)
{
    switch (ev.type)
    {
    case sf::Event::KeyPressed:
        if (ev.key.code == sf::Keyboard::Z)
            this->position[1] -= this->speed;
        else if (ev.key.code == sf::Keyboard::Q)
            this->position[0] -= this->speed;
        else if (ev.key.code == sf::Keyboard::S)
            this->position[1] += this->speed;
        else
            this->position[0] += this->speed;
    }
}