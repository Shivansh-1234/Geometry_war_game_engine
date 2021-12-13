#include "Game.h"

int main()
{
	srand(static_cast<unsigned>(time(NULL)));

	Game g("lololol.txt");

	g.run();

	return EXIT_SUCCESS;
}