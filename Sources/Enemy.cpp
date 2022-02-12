#include "./headers/Enemy.h"

void Enemy::initVariables()
{

	this->speed = 1.f;
}

void Enemy::initTexture()
{
	// Load a texture from file
	this->texture = new Texture();
	if (!this->texture->loadFromFile("../../Textures/red.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Coul not load texture from file \n";
	}
}

void Enemy::initAnimation()
{
	this->animationTimer.restart();
}

void Enemy::initSprite()
{
	// Set texture to sprite
	this->sprite.setTexture(*this->texture);
	this->sprite.scale(0.2f, 0.2f);
	/*
	this->currentFrame = IntRect(0, 0, 16, 16);

	this->sprite.setTextureRect(this->currentFrame);

	// Resize the Sprite
	this->sprite.scale(6.f, 6.f);
	*/
}

// Constructors/Deconstructors

Enemy::Enemy(float pos_x, float pos_y, float pos_x_center, float pos_y_center)
{
	this->initVariables();
	this->initTexture();
	this->initAnimation();
	this->initSprite();
	this->sprite.setPosition(pos_x, pos_y);
	this->posXCenter = pos_x_center;
	this->posYCenter = pos_y_center;

	float norm = sqrt((this->posXCenter - pos_x) * (this->posXCenter - pos_x) + (this->posYCenter - pos_y) * (this->posYCenter - pos_y));
	this->directionX = (this->posXCenter - pos_x) / norm;
	this->directionY = (this->posYCenter - pos_y) / norm;
}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete this->texture;
}

/*-------------------------------------*/
/*------------Accessors----------------*/
/*-------------------------------------*/
const FloatRect Enemy::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

Vector2f Enemy::getPosition()
{
    return this->sprite.getPosition();
}

Vector2f Enemy::getDirection()
{
	Vector2f dir = {this->directionX, this->directionY};
    return dir;
}
/*-------------------------------------*/
/*--------------Update-----------------*/
/*-------------------------------------*/
void Enemy::updateAnimation()
{
	if (this->animationTimer.getElapsedTime().asSeconds() >= 0.15f)
	{
		this->currentFrame.left = (this->currentFrame.left + 16) % 64;
		this->animationTimer.restart();
	}
	this->sprite.setTextureRect(this->currentFrame);
}

void Enemy::update()
{
	this->sprite.move(this->speed * this->directionX, this->speed * this->directionY);
	// std::cout << "Enemy is x : " << this->getBounds().left << ", y : " << this->getBounds().top << "\n";
	//this->updateAnimation();
}

void Enemy::render(RenderTarget *target)
{
	target->draw(this->sprite);
}