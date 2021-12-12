#pragma once

#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Window.hpp"
#include <iostream>

using namespace sf;

class Player
{
private:
    // Variables du joueur
    float moveSpeed;
    // Sprites
    Sprite sprite;
    Texture texture;
    // Orientattion

    // Private functions
    void initSprite();
    void initVariables();
    void initShape();
    void initTexture();

public:
    // Constructors and Destructors
    Player();
    virtual ~Player();

    // Functions
    void move(Event);
    void update();
    void renderPlayer(RenderTarget &);
    void move(const float, const float);

    // Accessors
    std::vector<float, float> getPosition();
    void getOrientation();
};
