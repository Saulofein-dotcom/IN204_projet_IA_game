#pragma once

#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Window.hpp"
#include <iostream>
#include <vector>
#include "Fireball.h"

using namespace sf;

class Player
{
private:
    // Fenêtre
    RenderWindow *window;

    // Variables du joueur
    float moveSpeed;
    std::vector<Fireball *> fireballs;
    float attackCoolDown;
    float attackCoolDownMax;

    // Sprites
    Sprite sprite;
    Texture texture;

    // Orientattion

    // Private functions
    void initSprite();
    void initVariables();
    void initTexture();
    void initCoolDowns();

public:
    // Constructors and Destructors
    Player();
    Player(RenderWindow *);
    virtual ~Player();

    // Functions
    void update();
    void updateFireballs();
    void updateAttack();
    const bool canAttack();
    void renderPlayer(RenderTarget &);
    void renderFireballs(RenderTarget &);
    void move(const float, const float);
    void shootFireballs();

    // Accessors
    std::vector<Fireball *> getFireballs();
    Vector2f getPosition();
    Vector2f getDirection();
};
