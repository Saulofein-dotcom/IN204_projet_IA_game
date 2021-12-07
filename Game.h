#ifndef GAME_H
#define GAME_H

#include<string>
#include<sstream>
#include "Player.h"
#include"Bullet.h"
#include"Enemy.h"
#include<map>

using namespace sf;

class Game
{
private:
	RenderWindow* window;

	//Resources
	std::map<std::string, Texture*> textures;
	std::vector<Bullet*> bullets;

	//GUI
	Font font;
	Text pointText;

	//World
	Texture worldBackgroundTexture;
	Sprite worldBackground;

	//System
	unsigned points;

	//Player
	Player* player;

	//Player GUI
	RectangleShape playerHpBar;
	RectangleShape playerHpBarBack;

	//Enemies
	float spawnTimer;
	float spawnTimerMax;
	std::vector<Enemy*> enemies;

	

	//Private functions
	void initWindow();
	void initTextures();
	void initGUI();
	void initWorld();
	void initSystem();
	void initPlayer();
	void initEnemies();

public:

	Game();
	~Game();
	
	//Functions
	void run();

	void updatePollEvents();
	void updateInput();
	void updateBullet();
	void updateGUI();
	void updateWorld();
	void updateCollision();
	void updateEnemies();
	void updateCombat();
	void update();
	void renderWorld();
	void renderGUI();
	void render();

};


#endif // !GAME_H

