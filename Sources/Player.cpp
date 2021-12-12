#include "./headers/Player.h"

void Player::initShape()
{
    this->shape.setFillColor(sf::Color::Green);
    this->shape.setSize(sf::Vector2f(100.f, 100.f));
    this->shape.setPosition(sf::Vector2f(100.f, 100.f));
}

void Player::initVariables()
{
    this->health = 100.f;   // vie du joueur
    this->moveSpeed = 10.f; // nombre de pixels parcoururs par appui sur une touche
}

void Player::initTexture()
{
    // Load texture from file
    if (!this->texture.loadFromFile("Textures/link_standing.png"))
    {
        std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture file" << std::endl;
    }
}

void Player::initSprite()
{
    // Set texture to sprite
    this->sprite.setTexture(this->texture);

    // Resize the sprite
    this->sprite.scale(0.8f, 0.8f);
}

Player::Player()
{
    this->initVariables();
    this->initTexture();
    this->initSprite();
}

Player::~Player()
{
}

void Player::renderPlayer(RenderTarget &target)
{
    target.draw(this->sprite);
}

void Player::move(const float dirX, const float dirY)
{
    this->sprite.move(this->moveSpeed * dirX, this->moveSpeed * dirY);
}

void Player::update()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
        this->move(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        this->move(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        this->move(0.f, 1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        this->move(1.f, 0.f);
    // Change position
}