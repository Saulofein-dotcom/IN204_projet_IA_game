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
#include<torch/torch.h>

#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

using namespace sf;
namespace T = torch;


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

    void update(uint a);
    void update(T::Tensor a);
    void update(double up, double left, double down, double right, double nothing);
    void updatePollEvents();
    void updateEnemies();
    void updatePlayer(T::Tensor a);
    void updatePlayer(uint a);
    void updatePlayer(double up, double left, double down, double right, double nothing);
    void updateGUI();
    void updateClock();

    void renderGUI();
    void render();
    void renderWorld();

    void triggerEndOfGame();

    auto actualState(uint nb_state);

    Image saveImage();
    T::Tensor imageToVectorC(unsigned width, unsigned height, Image myImage);

    auto step(double up, double left, double down, double right, double nothing, uint n_in);
    auto step(uint action , uint n_in, uint timestep);
    auto step( int actions, long n_in, long timestep, T::Tensor& actualState, int width, int height, int frames);

    void resetGame();

    void runTest();

    // Accessors
    std::vector<Enemy *> getEnemies();
};
