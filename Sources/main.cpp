#include "./headers/Game.h"
#include <time.h>

int main()
{
	/*
		Main function that starts the game
	*/
	srand(time(static_cast<unsigned>(0)));

	Game game;

	game.run();

	return 0;
}