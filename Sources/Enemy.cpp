#include "./headers/Enemy.h"
#include <SFML/Graphics/Texture.hpp>

/*-------------------------------------*/
/*-----------Init functions------------*/
/*-------------------------------------*/

void Enemy::initVariables()
{
	/*
	Initialize variables
	*/

	this->speed   = 5.f;
}

void Enemy::initTexture()
{
	/*
	Load a texture from file
	*/

	this->texture = new Texture();
	if (!this->texture->loadFromFile("../../Textures/Enemy.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture from file \n";
	}
}

void Enemy::initAnimation()
{
	/*
	Set the timer to 0
	*/

	this->animationTimer.restart();
}

void Enemy::initSprite()
{
	/*
	Set texture to sprite
	*/

	this->sprite.setTexture(*this->texture);
	this->currentFrame = IntRect(0,0,16,16);
	this->sprite.setTextureRect(this->currentFrame);

	//Resize the Sprite
	this->sprite.scale(6.f, 6.f);
}


/*-------------------------------------*/
/*-------------Cons/Dec----------------*/
/*-------------------------------------*/

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
	this->directionX = (this->posXCenter - pos_x)/norm;
	this->directionY = (this->posYCenter - pos_y)/ norm;
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


/*-------------------------------------*/
/*--------------Update-----------------*/
/*-------------------------------------*/

void Enemy::updateAnimation()
{
	/*
	Update animation of the enemy
	*/

	if(this->animationTimer.getElapsedTime().asSeconds() >= 0.15f)
	{
		this->currentFrame.left = (this->currentFrame.left + 16) % 64;	
		this->animationTimer.restart();
	}
	this->sprite.setTextureRect(this->currentFrame);
}

void Enemy::update()
{
	/*
	Update position of the enemy and its animation
	*/

    this->sprite.move(this->speed * this->directionX, this->speed * this->directionY);
	//std::cout << "Enemy is x : " << this->getBounds().left << ", y : " << this->getBounds().top << "\n";
	this->updateAnimation();
}


/*-------------------------------------*/
/*--------------Render-----------------*/
/*-------------------------------------*/

void Enemy::render(RenderTarget* target)
{
	target->draw(this->sprite);
}