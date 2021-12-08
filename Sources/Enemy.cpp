#include "./headers/Enemy.h"

void Enemy::initVariables()
{

	this->speed   = 10.f;
	this->hpMax   = 15;
	this->hp      = this->hpMax;
	this->damage  = 1;
	this->points  = 5;
}

void Enemy::initTexture()
{
	//Load a texture from file
	if (!this->texture.loadFromFile("Textures/ship.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Coul not load texture from file \n";
	}
}

void Enemy::initSprite()
{
	//Set texture to sprite
	this->sprite.setTexture(this->texture);

	//Resize the Sprite
	this->sprite.scale(0.1f, 0.1f);
}

// Constructors/Deconstructors

Enemy::Enemy()
{
    this->initVariables();
	this->initTexture();
	this->initSprite();
}

Enemy::~Enemy()
{
    
}