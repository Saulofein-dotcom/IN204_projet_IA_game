#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Window.hpp"
#include <vector>
#include <iostream>

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

    //Functions
    void initVariables();
    void initShape();
    void initTexture();
    void initSprite();

public:
    Enemy();
    ~Enemy();

    //Functions
	void update();
	void render(RenderTarget* target);

    


};