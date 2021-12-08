#include "./headers/Enemy.h"

void Enemy::initVariables()
{

	this->speed   = 2.f;
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

Enemy::Enemy(float pos_x, float pos_y, float pos_x_center, float pos_y_center)
{
    this->initVariables();
	this->initTexture();
	this->initSprite();
    this->sprite.setPosition(pos_x, pos_y);
    this->posXCenter = pos_x_center;
    this->posYCenter = pos_y_center;

	float norm = sqrt((this->posXCenter - pos_x) * (this->posXCenter - pos_x) + (this->posYCenter - pos_y) + (this->posYCenter - pos_y));
	this->directionX = (this->posXCenter - pos_x)/norm;
	this->directionY = (this->posYCenter - pos_y)/ norm;
}

Enemy::~Enemy()
{
    
}

/*-------------------------------------*/
/*------------Accessors----------------*/
/*-------------------------------------*/
const FloatRect Enemy::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

void Enemy::update()
{
    this->sprite.move(this->speed * this->directionX, this->speed * this->directionY);
	std::cout << "Enemy is x : " << this->getBounds().left << ", y : " << this->getBounds().top << "\n";
}

void Enemy::render(RenderTarget* target)
{
	target->draw(this->sprite);
}