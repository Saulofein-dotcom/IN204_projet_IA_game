#pragma once

#include "Player.h"
#include "Enemy.h"
#include "EnemyRock.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

using namespace sf;

class Game
{
private:
    // Fenêtre
    RenderWindow *window;

    // World
    Texture worldBackgroundTexture;
    Sprite worldBackground;

    // Player
    Player *player;

    // Enemies
    float spawnTimer;
    float spawnTimerMax;
    std::vector<Enemy *> enemies;

    float spawnTimerRock;
    float spawnTimerMaxRock;
    std::vector<EnemyRock *> enemiesRock;

    // GUI
    sf::Font font;
    sf::Text timeText;

    // Time
    Clock clock;
    Time elapsedTime;

    // Fonctions privées
    void initWindow();
    void initWorld();
    void initPlayer();
    void initEnemies();
    void initGUI();
    void initClock();

public:
    Game();
    ~Game();

    // Fonctions
    void run();

    void update();
    void updatePollEvents();
    void updateEnemies();
    void updatePlayer();
    void updateGUI();
    void updateClock();

    void renderGUI();
    void render();
    void renderWorld();

    // Accessors
    std::vector<Enemy *> getEnemies();
};
