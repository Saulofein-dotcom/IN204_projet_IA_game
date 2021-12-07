#pragma once
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Window.hpp"
#include <vector>

class Player
{
private:
    // Variables du joueur
    std::vector<int, int> position;
    int health;
    int speed;
    // Sprites
    // Orientattion

    // Private functions
    void initVariables();

public:
    // Constructors and Destructors
    Player();
    virtual ~Player();

    // Functions
    void move(sf::Event);

    // Accessors
    std::vector<int, int> getPosition();
    void getOrientation();
};
