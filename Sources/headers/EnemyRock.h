#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Enemy.h"
#include <vector>
#include <iostream>
#include <math.h>



using namespace sf;

class EnemyRock
{
protected:

    //Rock
    Sprite sprite;
	Texture* texture;
    Clock animationTimer;

    //Shadow
    Sprite spriteShadow;
    Texture* textureShadow;
    Clock animationTimerShadow;

    
    //Rock attributes
    float speed;

    //Shadow attributes
    float posXExplode;
    float posYExplode;

    //Triggers
    bool exploded;
    bool toDestroy;
    bool isDangerous;

    //Animation
    IntRect currentFrame;
    IntRect currentFrameShadow;

    //Functions
    void initVariables();
    void initShape();
    void initTexture();
    void initAnimation();
    void initSprite();

public:
    //Cons/Dec
    EnemyRock();
    EnemyRock(float pos_x, float pos_y, float pos_x_explode, float pos_y_explode);
    ~EnemyRock();

    //Accessors
	const FloatRect getBounds() const;
    float getPosYExplode() const;
    bool isDestroyed() const;

    //Functions
    void updateAnimation();
	void update();

    void render(RenderTarget* target);

    


};