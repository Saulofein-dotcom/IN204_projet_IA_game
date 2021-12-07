#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include<iostream>


using namespace sf;

class Player
{
private:
	Sprite sprite;
	Texture texture;

	int hp;
	int hpMax;

	float movementSpeed;
	float attackCooldown;
	float attackCooldownMax;

	//Private functions
	void initVariables();
	void initTexture();
	void initSprite();

public:
	Player();
	~Player();

	//Accessor
	const Vector2f& getPos() const;
	const FloatRect getBounds() const;
	const int& getHp() const;
	const int& getHpMax() const;

	//Modifiers
	void setPosition(const Vector2f pos);
	void setPosition(const float x, const float y);
	void setHp(const int hp);
	void loseHp(const int value);

	//Functions
	void move(const float dirX, const float dirY);
	const bool canAttack();

	void update();
	void updateAttack();
	void render(RenderTarget& target);
};

#endif // !PLAYER_H
