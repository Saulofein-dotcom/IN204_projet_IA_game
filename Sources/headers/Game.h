#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"

using namespace sf;

class Game
{
private:
    // Fenêtre
    RenderWindow *window;

    // Variables
    Player *player;

    // Fonctions privées
    void initWindow();
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
#endif // !GAME_H