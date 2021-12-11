#include "./headers/EnemyRock.h"

void EnemyRock::initVariables()
{

	this->speed   = 6.f;
}

void EnemyRock::initTexture()
{
	//Load a texture from file
	this->texture = new Texture();
	if (!this->texture->loadFromFile("Textures/rock_air.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture from file \n";
	}

	this->textureShadow = new Texture();
	if (!this->textureShadow->loadFromFile("Textures/rockShadow.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Coul not load texture from file \n";
	}
}

void EnemyRock::initAnimation()
{
	this->animationTimer.restart();
	this->animationTimerShadow.restart();
}

void EnemyRock::initSprite()
{
	//Set texture to sprite
	this->sprite.setTexture(*this->texture);

	this->currentFrame = IntRect(0,0,150,150);

	this->sprite.setTextureRect(this->currentFrame);

	//Resize the Sprite
	this->sprite.scale(1.f, 1.f);

	//Set texture to sprite
	this->spriteShadow.setTexture(*this->textureShadow);

	this->currentFrameShadow = IntRect(0,0,150,48);

	this->spriteShadow.setTextureRect(this->currentFrameShadow);

	//Resize the Sprite
	this->spriteShadow.scale(1.f, 1.f);
}

EnemyRock::EnemyRock(float pos_x, float pos_y, float pos_x_explode, float pos_y_explode)
{
	this->initVariables();
	this->initTexture();
	this->initAnimation();
	this->initSprite();
	this->posXExplode = pos_x_explode;
	this->posYExplode = pos_y_explode;
	this->spriteShadow.setPosition(this->posXExplode, this->posYExplode + 120);
    this->sprite.setPosition(pos_x, pos_y);

}

EnemyRock::~EnemyRock()
{
	delete this->textureShadow;
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

	if(this->animationTimerShadow.getElapsedTime().asSeconds() >= 0.2f && this->currentFrameShadow.left < 750)
	{
		this->currentFrameShadow.left = (this->currentFrameShadow.left + 150);	
		this->animationTimerShadow.restart();
	}

	if(this->currentFrameShadow.left >= 900) this->animationTimerShadow.restart();

	this->spriteShadow.setTextureRect(this->currentFrameShadow);
}

void EnemyRock::render(RenderTarget* target)
{
	target->draw(this->sprite);
	target->draw(this->spriteShadow);
}

