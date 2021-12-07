#ifndef ENEMY_H
#define ENEMY_H

#include "SFML/Graphics.hpp"

using namespace sf;

class Enemy
{
private:
	unsigned pointCount;
	CircleShape shape;
	int type;
	float speed;
	int hp;
	int hpMax;
	int damage;
	int points;

	void initVariables();
	void initShape();
	
public:
	Enemy(float posX, float posY);
	~Enemy();

	//Accessors
	const FloatRect getBounds() const;
	const int& getPoints() const;
	const int& getDamage() const;

	//Functions
	void update();
	void render(RenderTarget* target);
};

#endif // !ENEMY_H
