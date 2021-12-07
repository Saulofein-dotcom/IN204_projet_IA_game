#include "Player.h"

void Player::initVariables()
{
	this->hpMax = 20;
	this->hp = this->hpMax;

	this->movementSpeed = 10.f;

	this->attackCooldownMax = 10.f;
	this->attackCooldown = this->attackCooldownMax;
}

void Player::initTexture()
{
	//Load a texture from file
	if (!this->texture.loadFromFile("Textures/ship.png"))
	{
		std::cout << "ERROR::PLAYER::INITTEXTURE::Coul not load texture from file \n";
	}
}

void Player::initSprite()
{
	//Set texture to sprite
	this->sprite.setTexture(this->texture);

	//Resize the Sprite
	this->sprite.scale(0.1f, 0.1f);
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

const Vector2f & Player::getPos() const
{
	return this->sprite.getPosition();
}

const FloatRect Player::getBounds() const
{
	return this->sprite.getGlobalBounds();
}

const int & Player::getHp() const
{
	return this->hp;
}

const int & Player::getHpMax() const
{
	return this->hpMax;
}


void Player::setPosition(const Vector2f pos)
{
	this->sprite.setPosition(pos);
}

void Player::setPosition(const float x, const float y)
{
	this->sprite.setPosition(x, y);
}

void Player::setHp(const int hp)
{
	this->hp = hp;
}

void Player::loseHp(const int value)
{
	this->hp -= value;
	if (this->hp < 0) this->hp = 0;

}

void Player::move(const float dirX, const float dirY)
{
	this->sprite.move(this->movementSpeed * dirX, this->movementSpeed * dirY);
}

const bool Player::canAttack()
{
	if (this->attackCooldown >= this->attackCooldownMax)
	{
		this->attackCooldown = 0.f;
			return true;
	}
	return false;
}

//Functions
void Player::update()
{
	this->updateAttack();
}

void Player::updateAttack()
{
	if (this->attackCooldown < this->attackCooldownMax)
	{
		this->attackCooldown += 0.5f;
	}
}

void Player::render(RenderTarget& target)
{
	target.draw(this->sprite);
}
