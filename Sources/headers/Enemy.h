#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>
#include <math.h>

using namespace sf;

class Enemy
{
protected:
    //Enemy
    Sprite sprite;
    Texture *texture;
    Clock animationTimer;

    //Attributes - an enemy has an initial position and a direction
    float speed;
    float posXCenter;
    float posYCenter;
    float directionX;
    float directionY;

    // Animation
    IntRect currentFrame;

    // Functions
    void initVariables();
    void initShape();
    void initTexture();
    void initAnimation();
    void initSprite();

public:

    //Cons/Dec
    Enemy();
    Enemy(float pos_x, float pos_y, float pos_x_center, float pos_y_center);
    ~Enemy();

<<<<<<< HEAD
    //Accessors
	const FloatRect getBounds() const;
=======
    // Accessors
    const FloatRect getBounds() const;
>>>>>>> romain

    // Functions
    void updateAnimation();
    void update();
    void render(RenderTarget *target);
};