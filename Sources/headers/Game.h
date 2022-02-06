#pragma once
#include "Player.h"
#include "Enemy.h"
#include "EnemyRock.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include<vector>
#include <algorithm>

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
    std::vector<Enemy *> enemies; // Skeleton enemies

    float spawnTimerRock;
    float spawnTimerMaxRock;
    std::vector<EnemyRock *> enemiesRock; // Rock enemies

    // GUI
    sf::Font font;
    sf::Text timeText;

    sf::Text gameOverText; // Unused because not useful for AI
    bool end = false;

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

    void update(int a);
    void updatePollEvents();
    void updateEnemies();
    void updatePlayer(int a);
    void updateGUI();
    void updateClock();

    void renderGUI();
    void render();
    void renderWorld();

    void triggerEndOfGame();

    Image saveImage();
    std::vector<unsigned> imageToVectorC(unsigned width, unsigned height, Image myImage);

    auto step(int action, int stackFrame);

    // Accessors
    std::vector<Enemy *> getEnemies();
};
