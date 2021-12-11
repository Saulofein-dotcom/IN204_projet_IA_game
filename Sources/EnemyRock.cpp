#include "./headers/EnemyRock.h"

void EnemyRock::initVariables()
{

	this->speed   = 5.f;
}

void EnemyRock::initTexture()
{
	//Load a texture from file
	this->texture = new Texture();
	if (!this->texture->loadFromFile("Textures/rock_air.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Coul not load texture from file \n";
	}
}

void EnemyRock::initAnimation()
{
	this->animationTimer.restart();
}

void EnemyRock::initSprite()
{
	//Set texture to sprite
	this->sprite.setTexture(*this->texture);

	this->currentFrame = IntRect(0,0,150,150);

	this->sprite.setTextureRect(this->currentFrame);

	//Resize the Sprite
	this->sprite.scale(1.f, 1.f);
}

EnemyRock::EnemyRock(float pos_x, float pos_y, float pos_x_explode, float pos_y_explode)
{
	this->initVariables();
	this->initTexture();
	this->initAnimation();
	this->initSprite();
    this->sprite.setPosition(pos_x, pos_y);
	this->posXExplode = pos_x_explode;
	this->posYExplode = pos_y_explode;

}

EnemyRock::~EnemyRock()
{
    delete this->texture;
}

/*-------------------------------------*/
/*------------Accessors----------------*/
/*-------------------------------------*/
const FloatRect EnemyRock::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

float EnemyRock::getPosYExplode() const
{
	return this->posYExplode;
}
/*-------------------------------------*/
/*--------------Update-----------------*/
/*-------------------------------------*/

void EnemyRock::update()
{
    this->sprite.move(0, this->speed);
	//std::cout << "Enemy is x : " << this->getBounds().left << ", y : " << this->getBounds().top << "\n";
	this->updateAnimation();
}


void EnemyRock::updateAnimation()
{
	if(this->animationTimer.getElapsedTime().asSeconds() >= 1.f)
	{
		this->currentFrame.left = (this->currentFrame.left + 150) % 450;	
		this->animationTimer.restart();
	}
	this->sprite.setTextureRect(this->currentFrame);
}

void EnemyRock::render(RenderTarget* target)
{
	target->draw(this->sprite);
}

