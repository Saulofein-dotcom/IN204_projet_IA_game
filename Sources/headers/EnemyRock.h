#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Window.hpp"
#include "Enemy.h"
#include <vector>
#include <iostream>
#include <math.h>

using namespace sf;

class EnemyRock
{

    /*
        The EnemyRock class implements the behavior of the rocks falling on the ground. The EnemyRock is dangerous for the players a short time before it hits the ground. When the player hits a rock, the game ends.
    */

protected:
    Sprite sprite;
    Texture *texture;

    Sprite spriteShadow;
    Texture *textureShadow;
    Clock animationTimerShadow;

    Clock animationTimer;

    float speed;

    float posXExplode;
    float posYExplode;

    bool exploded;
    bool toDestroy;
    bool dangerous;

    // Animation
    IntRect currentFrame;
    IntRect currentFrameShadow;

    // Functions
    void initVariables();
    void initShape();
    void initTexture();
    void initAnimation();
    void initSprite();

public:
    EnemyRock();
    EnemyRock(float pos_x, float pos_y, float pos_x_explode, float pos_y_explode);
    ~EnemyRock();

    // Accessors
    const FloatRect getBounds() const;
    float getPosYExplode() const;
    bool isDestroyed() const;
    bool isDangerous() const;

    // Functions
    void updateAnimation();
    void updateDangerous();
    void update();

    void render(RenderTarget *target);
};