
#include "pch.h"
#include "game.h"

int main() {
	START_PROFILE;
	
	// setup
	Textures::init();
	Game game;
	game.mainMenu();


	END_PROFILE;
	std::cout << "\n[Program Finished]";
	std::cin.get();
}

