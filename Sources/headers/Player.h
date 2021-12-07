
#include "../../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include "../../SFML-2.5.1/include/SFML/Window.hpp"
#include <vector>

class Player
{
private:
    // Variables du joueur
    int x, y;
    float health;
    float speed;
    sf::RectangleShape shape;
    // Sprites
    // Orientattion

    // Private functions

public:
    // Constructors and Destructors
    Player();
    virtual ~Player();

    // Functions
    void move(sf::Event);
    void initVariables();
    void initShape();
    void updatePosition();
    void renderPlayer();

    // Accessors
    std::vector<float, float> getPosition();
    void getOrientation();
};
