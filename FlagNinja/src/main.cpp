
#include "pch.h"
#include "game.h"

int main() {
	
	// setup
	Textures::init();
	Game game;
	game.mainMenu();


	std::cout << "\n[Program Finished]";
	std::cin.get();
}

