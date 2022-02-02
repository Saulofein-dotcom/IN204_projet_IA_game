#pragma once
<<<<<<< HEAD

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
=======
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Window.hpp"
>>>>>>> romain
#include <iostream>
#include <vector>
#include "Fireball.h"
#include <math.h>
#include <iostream>

using namespace sf;

class Player
{
private:
    // Fenêtre
    RenderWindow *window;

    // Variables du joueur
    float moveSpeed;
    float attackCoolDown;
    float attackCoolDownMax;
    Vector2f mouseDirection;

    // Sprites
    Sprite sprite;
    Texture texture;
    Sprite swordSprite;
    Texture swordTexture;

    // Private functions
    void initSprite();
    void initVariables();
    void initTexture();
    void initCoolDowns();
    void initPosition();

public:
    // Constructors and Destructors
    Player();
    Player(RenderWindow *);
    virtual ~Player();

    // Public Variables
    std::vector<Fireball *> fireballs;

    // Functions
    void update();
    void updateFireballs();
    void updateAttack();
    void updateSwordRotation();
    void updateSwordPosition();
    const bool canAttack();
    void renderPlayer(RenderTarget &);
    void renderFireballs(RenderTarget &);
    void move(const float, const float);
    void shootFireballs();

    // Setters
    void setPosition(float, float);
    void setPosition(sf::Vector2f);

    // Getters
    std::vector<Fireball *> getFireballs();
    Vector2f getPosition();
    Vector2f getMouseDirection();
    float getRotationAngle(float, float);
    const FloatRect getBounds() const;
};
