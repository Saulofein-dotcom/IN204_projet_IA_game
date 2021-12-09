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
    Clock animationTimer;

    float speed;
	int hp;
	int hpMax;
	int damage;
	int points;

    float posXCenter;
    float posYCenter;

    float directionX;
    float directionY;

    //Animation
    IntRect currentFrame;

    //Functions
    void initVariables();
    void initShape();
    void initTexture();
    void initAnimation();
    void initSprite();

public:
    Enemy(float pos_x, float pos_y, float pos_x_center, float pos_y_center);
    ~Enemy();

    //Accessors
	const FloatRect getBounds() const;

    //Functions
    void updateAnimation();
	void update();
	void render(RenderTarget* target);

    


};