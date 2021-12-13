#include "./headers/Fireball.h"

void Fireball::initVariables()
{
    this->speed = 15.f;
}

void Fireball::initTexture()
{
    // Load texture from file
    if (!this->texture.loadFromFile("Textures/fireball.png"))
    {
        std::cout << "ERROR::FIREBALL::INITTEXTURE::Could not load texture file" << std::endl;
    }
}

void Fireball::initSprite()
{
    // Set texture to sprite
    this->sprite.setTexture(this->texture);

    // Resize the sprite
    this->sprite.scale(1.f, 1.f);
}

Fireball::Fireball()
{
}

Fireball::Fireball(float posX, float posY, float dirX, float dirY)
{
    this->initTexture();
    this->initSprite();
    this->sprite.setPosition(posX, posY);
    this->direction.x = dirX;
    this->direction.y = dirY;
}

Fireball::~Fireball()
{
}

void Fireball::move()
{
    this->sprite.move(this->direction * this->speed);
}

void Fireball::updateFireball()
{
    // Movement
    this->move();
}

void Fireball::renderFireball(RenderTarget &target)
{
    target.draw(this->sprite);
}

// Accessors
const FloatRect Fireball::getBounds() const
{
    return this->sprite.getGlobalBounds();
}