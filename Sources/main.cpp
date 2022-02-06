#include "./headers/Game.h"
#include <time.h>

int main()
{
	srand(time(static_cast<unsigned>(0)));

	Game game;

	std::vector<unsigned> a = {2, 3, 4};
	std::vector<unsigned> b(10);
	std::copy(a.begin(), a.end(), b.begin());

	std::cout << b[0] << b[1] << std::endl;

	game.run();

	return 0;
}