#pragma once

#include "Player.h"
#include "Enemy.h"
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

using namespace sf;

class Game
{
private:
    // Fenêtre
    RenderWindow *window;

    // Variables
    Player *player;

    //Enemies
	float spawnTimer;
	float spawnTimerMax;
	std::vector<Enemy*> enemies;

    // Fonctions privées
    void initWindow();
    void initPlayer();
    void initVariables();

public:
    Game();
    ~Game();

    // Fonctions
    void run();

    void update();
    void updatePollEvents();

    void render();
};
