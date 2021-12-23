#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Window.hpp"
#include <vector>
#include <iostream>
#include <math.h>

using namespace sf;

class Fireball
{
private:
    // Variables du projectile
    float speed;
    Vector2f direction;

    // Sprites
    Sprite sprite;
    Texture texture;

    // Private functions
    void initSprite();
    void initVariables();
    void initTexture();

public:
    // Constructors and destructors
    Fireball();
    Fireball(float posX, float posY, float dirX, float dirY);
    virtual ~Fireball();

    // Functions
    void move();
    void updateFireball();
    void renderFireball(RenderTarget &);
    float getRotationAngle(float, float);

    // Getters
    const FloatRect getBounds() const;
};