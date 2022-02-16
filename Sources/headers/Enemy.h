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
    /*
        The Enemy class implements skeletons enemies who can hit the player and end the game during a collision. Their behavior is straightforward : they move in a line and are deleted when they are out of borders.
    */

protected:
    Sprite sprite;
    Texture *texture;
    Clock animationTimer;

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
    Enemy();
    Enemy(float pos_x, float pos_y, float pos_x_center, float pos_y_center);
    ~Enemy();

    // Accessors
    const FloatRect getBounds() const;

    // Functions
    void updateAnimation();
    void update();
    void render(RenderTarget *target);
};