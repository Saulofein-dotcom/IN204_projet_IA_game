#include "./headers/Fireball.h"

void Fireball::initVariables()
{
    this->speed = 20.f;
}

void Fireball::initTexture()
{
    // Load texture from file
    if (!this->texture.loadFromFile("../../Textures/fireball.png"))
    {
        std::cout << "ERROR::FIREBALL::INITTEXTURE::Could not load texture file" << std::endl;
    }
}

void Fireball::initSprite()
{
    // Set texture to sprite
    this->sprite.setTexture(this->texture);

    // Resize the sprite
    this->sprite.scale(2.f, 2.f);

    // Set the origin of the fireball to its middle bottom
    this->sprite.setOrigin(0, this->sprite.getLocalBounds().height / 2);
}

Fireball::Fireball()
{
}

Fireball::Fireball(float posX, float posY, float dirX, float dirY)
{
    this->initVariables();
    this->initTexture();
    this->initSprite();
    this->sprite.setRotation(this->getRotationAngle(dirX, dirY));
    this->sprite.setPosition(posX, posY);
    this->direction.x = dirX;
    this->direction.y = dirY;
}

Fireball::~Fireball()
{
}

// Functions
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

float Fireball::getRotationAngle(float X, float Y)
{
    if (Y > 0)
        return std::acos(X) * 180.0f / 3.1415;
    else
        return std::acos(-X) * 180.0f / 3.1415 + 180.f;
}

// Accessors
const FloatRect Fireball::getBounds() const
{
    return this->sprite.getGlobalBounds();
}