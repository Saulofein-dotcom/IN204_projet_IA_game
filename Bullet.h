#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics.hpp"
#include<iostream>

using namespace sf;

class Bullet
{
private:
	Sprite shape;

	Vector2f direction;
	float movementSpeed;

public:
	Bullet();
	Bullet(Texture* texture, float posX, float posY, float dirX, float dirY, float movementSpeed);
	~Bullet();

	//Accessors
	const FloatRect getBounds() const ;

	//Functions
	void update();
	void render(RenderTarget* target);
	
};

#endif // !BULLET_H