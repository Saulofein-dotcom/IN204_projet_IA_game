#include "Enemy.h"


void Enemy::initVariables()
{
	this->pointCount = rand() % 8 + 3;
	this->type    = 0;
	this->speed   = static_cast<float>(this->pointCount/2);
	this->hpMax   = static_cast<int>(this->pointCount);
	this->hp      = this->hpMax;
	this->damage  = this->pointCount;
	this->points  = this->pointCount;
}

void Enemy::initShape()
{
	this->shape.setRadius(this->pointCount*3);
	this->shape.setPointCount(this->pointCount);
	this->shape.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
}

Enemy::Enemy(float posX, float posY)
{
	this->initVariables();
	this->initShape();
	this->shape.setPosition(posX, posY);
}

Enemy::~Enemy()
{
}

const FloatRect Enemy::getBounds() const
{
	return this->shape.getGlobalBounds();
}

const int & Enemy::getPoints() const
{
	return this->points;
}

const int & Enemy::getDamage() const
{
	return this->damage;
}

void Enemy::update()
{
	this->shape.move(0.f, this->speed);
}

void Enemy::render(RenderTarget * target)
{
	target->draw(this->shape);
}
