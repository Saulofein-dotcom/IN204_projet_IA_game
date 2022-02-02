#include "./headers/EnemyRock.h"

/*-------------------------------------*/
/*-----------Init functions------------*/
/*-------------------------------------*/



void EnemyRock::initVariables()
{
<<<<<<< HEAD
	/*
	Initialize variables
	*/
	this->speed   = 6.f;
=======

	this->speed = 6.f;
>>>>>>> romain
	this->exploded = false;
	this->toDestroy = false;
	this->dangerous = false;
}

void EnemyRock::initTexture()
{
<<<<<<< HEAD
	/*
	Load a texture from file
	*/

	//For the rock
=======
	// Load a texture from file
>>>>>>> romain
	this->texture = new Texture();
	if (!this->texture->loadFromFile("../../Textures/rock_air.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture from file \n";
	}

	//For the shadow
	this->textureShadow = new Texture();
	if (!this->textureShadow->loadFromFile("../../Textures/rockShadow.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture from file \n";
	}
}

void EnemyRock::initAnimation()
{
	/*
	Initialize the timers to 0
	*/

	this->animationTimer.restart();
	this->animationTimerShadow.restart();
}

void EnemyRock::initSprite()
{
<<<<<<< HEAD
	/*
	Set texture to sprite
	*/

	//For the rock
	this->sprite.setTexture(*this->texture);
	this->currentFrame = IntRect(0,0,150,150);
	this->sprite.setTextureRect(this->currentFrame);

	//For the shadow
	this->spriteShadow.setTexture(*this->textureShadow);
	this->currentFrameShadow = IntRect(0,0,150,48);
	this->spriteShadow.setTextureRect(this->currentFrameShadow);

=======
	// Set texture to sprite
	this->sprite.setTexture(*this->texture);

	this->currentFrame = IntRect(0, 0, 150, 150);

	this->sprite.setTextureRect(this->currentFrame);

	// Resize the Sprite
	this->sprite.scale(1.f, 1.f);

	// Set texture to sprite
	this->spriteShadow.setTexture(*this->textureShadow);

	this->currentFrameShadow = IntRect(0, 0, 150, 48);

	this->spriteShadow.setTextureRect(this->currentFrameShadow);

	// Resize the Sprite
	this->spriteShadow.scale(1.f, 1.f);
>>>>>>> romain
}


/*-------------------------------------*/
/*-------------Cons/Dec----------------*/
/*-------------------------------------*/

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

bool EnemyRock::isDestroyed() const
{
	return this->toDestroy;
}

<<<<<<< HEAD

=======
bool EnemyRock::isDangerous() const
{
	return this->dangerous;
}
>>>>>>> romain
/*-------------------------------------*/
/*--------------Update-----------------*/
/*-------------------------------------*/

void EnemyRock::update()
{
<<<<<<< HEAD
	/*
	Update position of the rock, its animation and if it is dangerous for the player
	*/

    if(!this->exploded) this->sprite.move(0, this->speed);
	if(this->sprite.getPosition().y + 80 > this->getPosYExplode()) isDangerous = true;
	//std::cout << "Enemy is x : " << this->getBounds().left << ", y : " << this->getBounds().top << "\n";
	this->updateAnimation();
=======
	// Move rock enemy
	if (!this->exploded)
		this->sprite.move(0, this->speed);

	if (this->sprite.getPosition().y + 100 > this->getPosYExplode())
		// std::cout << "Aie";
		// std::cout << "Enemy is x : " << this->getBounds().left << ", y : " << this->getBounds().top << "\n";
		this->updateAnimation();

	// Update dangerous
	this->updateDangerous();
>>>>>>> romain
}

void EnemyRock::updateDangerous()
{
	if (this->sprite.getPosition().y + 50 >= this->getPosYExplode())
		this->dangerous = true;
}

void EnemyRock::updateAnimation()
{
<<<<<<< HEAD
	/*
	Update animation of the rock
	*/

	//For the shadow
	if(this->animationTimerShadow.getElapsedTime().asSeconds() >= 0.2f && this->currentFrameShadow.left < 750 && !this->exploded)
=======
	if (this->animationTimerShadow.getElapsedTime().asSeconds() >= 0.2f && this->currentFrameShadow.left < 750 && !this->exploded)
>>>>>>> romain
	{
		this->currentFrameShadow.left = (this->currentFrameShadow.left + 150);
		this->animationTimerShadow.restart();
	}
<<<<<<< HEAD
	this->spriteShadow.setTextureRect(this->currentFrameShadow);


	//For the rock
	if(this->animationTimer.getElapsedTime().asSeconds() >= 1.f  && !this->exploded && this->getBounds().top < this->getPosYExplode())
	{
		//While the rock is not touching the ground
		this->currentFrame.left = (this->currentFrame.left + 150) % 450;	
		this->animationTimer.restart();
	}
	else if(this->getBounds().top >= this->getPosYExplode() && !this->exploded) 
=======
	if (this->currentFrameShadow.left >= 900)
		this->animationTimerShadow.restart();
	this->spriteShadow.setTextureRect(this->currentFrameShadow);

	if (this->animationTimer.getElapsedTime().asSeconds() >= 1.f && !this->exploded && this->getBounds().top < this->getPosYExplode())
	{
		this->currentFrame.left = (this->currentFrame.left + 150) % 450;
		this->animationTimer.restart();
	}

	else if (this->getBounds().top >= this->getPosYExplode() && !this->exploded)
>>>>>>> romain
	{
		//When the rock touch the ground, the variable "exploded" become true and we begin the animation of explosion
		exploded = true;
		this->currentFrame.top = 150;
		this->currentFrame.left = 0;
		this->animationTimer.restart();
	}
	else if (this->exploded && this->currentFrame.left < 1050 && this->animationTimer.getElapsedTime().asSeconds() >= 0.06f)
	{
		//While the rock is exploding
		this->currentFrame.left = (this->currentFrame.left + 150);
		this->currentFrameShadow.left > 150 ? this->currentFrameShadow.left = this->currentFrameShadow.left - 300 : this->currentFrameShadow.left = 0;
		this->animationTimer.restart();
	}
<<<<<<< HEAD
	else if(this->exploded && this->currentFrame.left >= 1050) 
=======

	else if (this->exploded && this->currentFrame.left >= 1050)
>>>>>>> romain
	{
		//The variable "toDestroy" become true that mean we have to remove the rock from the game
		this->toDestroy = true;
	}
	this->sprite.setTextureRect(this->currentFrame);
}
<<<<<<< HEAD


/*-------------------------------------*/
/*--------------Render-----------------*/
/*-------------------------------------*/
=======
>>>>>>> romain

void EnemyRock::render(RenderTarget *target)
{
	target->draw(this->spriteShadow);
	target->draw(this->sprite);
}
