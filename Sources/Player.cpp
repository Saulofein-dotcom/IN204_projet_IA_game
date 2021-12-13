#include "./headers/Player.h"

// Init functions
void Player::initVariables()
{
    this->moveSpeed = 10.f; // nombre de pixels parcoururs par appui sur une touche
    this->attackCoolDownMax = 10.f;
    this->attackCoolDown = this->attackCoolDownMax;
}

void Player::initTexture()
{
    // Load texture from file
    if (!this->texture.loadFromFile("Textures/link_standing.png"))
    {
        std::cout << "ERROR::PLAYER::INITTEXTURE::Could not load texture file" << std::endl;
    }
}

void Player::initSprite()
{
    // Set texture to sprite
    this->sprite.setTexture(this->texture);

    // Resize the sprite
    this->sprite.scale(0.8f, 0.8f);
}

// Constructors and destructors
Player::Player()
{
    this->initVariables();
    this->initTexture();
    this->initSprite();
}

Player::Player(RenderWindow *window)
{
    this->window = window;
    this->initVariables();
    this->initTexture();
    this->initSprite();
}

Player::~Player()
{
    // Delete fireballs
    for (auto *fireball : this->fireballs)
        delete fireball;
}

// Functions
void Player::renderPlayer(RenderTarget &target)
{
    target.draw(this->sprite);
}

void Player::renderFireballs(RenderTarget &target)
{
    for (auto *fireball : this->fireballs)
    {
        fireball->renderFireball(target);
    }
}

void Player::move(const float dirX, const float dirY)
{
    this->sprite.move(this->moveSpeed * dirX, this->moveSpeed * dirY);
}

void Player::shootFireballs()
{
    Vector2f direction = this->getDirection();
    Vector2f position = this->getPosition();
    this->fireballs.push_back(new Fireball(position.x, position.y, 0.f, 1.f));
}

void Player::updateFireballs()
{
    /*
        Déplacement des fireballs et destruction lorsqu'elles atteignent
        le bord de la fenêtre
    */
    unsigned counter = 0;
    for (auto *fireball : this->fireballs)
    {
        std::cout << this->fireballs.size() << std::endl;
        fireball->updateFireball(); // Mouvement fireball
        if (fireball->getBounds().top + fireball->getBounds().height < 0.f)
        {
            // Delete fireball
            delete this->fireballs.at(counter);
            this->fireballs.erase(this->fireballs.begin() + counter);
            --counter;
        }

        ++counter;
    }
    std::cout << "Counter final : " << counter << std::endl;
}

void Player::updateAttack()
{
    if (this->attackCoolDown < this->attackCoolDownMax)
        this->attackCoolDown += 1.f;
}

void Player::update()
{
    // Déplacement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
        this->move(0.f, -1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        this->move(-1.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        this->move(0.f, 1.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        this->move(1.f, 0.f);

    // Tire de fireball
    if (Mouse::isButtonPressed(Mouse::Left) && this->canAttack())
    {
        std::cout << "PAN !" << std::endl;
        this->shootFireballs();
    }
    this->updateAttack();
    // Mouvement des fireballs
    this->updateFireballs();
}

const bool Player::canAttack()
{
    if (this->attackCoolDown >= attackCoolDownMax)
    {
        this->attackCoolDown = 0.f;
        return true;
    }
    return false;
}

// Accessors
std::vector<Fireball *> Player::getFireballs()
{
    return this->fireballs;
}

Vector2f Player::getPosition()
{
    return this->sprite.getPosition();
}

Vector2f Player::getDirection()
{
    float mouseX = Mouse::getPosition().x;
    float mouseY = Mouse::getPosition().y;
    Vector2f direction(mouseX / this->window->getSize().x, mouseY / this->window->getSize().y);
    return direction;
}
