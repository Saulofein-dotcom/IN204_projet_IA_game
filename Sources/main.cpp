#include "./headers/Game.h"
#include<time.h>
//#include<torch/torch.h>

int main()
{
	srand(time(static_cast<unsigned>(0)));

	Game game;

	game.run();

	return 0;
}