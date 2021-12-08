#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Window.hpp"
#include <vector>
#include <iostream>
#include <math.h>

using namespace sf;

class Enemy
{
private:
    Sprite sprite;
	Texture texture;

    float speed;
	int hp;
	int hpMax;
	int damage;
	int points;

    float posXCenter;
    float posYCenter;

    float directionX;
    float directionY;

    //Functions
    void initVariables();
    void initShape();
    void initTexture();
    void initSprite();

public:
    Enemy(float pos_x, float pos_y, float pos_x_center, float pos_y_center);
    ~Enemy();

    //Accessors
	const FloatRect getBounds() const;

    //Functions
	void update();
	void render(RenderTarget* target);

    


};