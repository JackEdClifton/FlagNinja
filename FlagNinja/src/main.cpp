
#include "pch.h"
#include "game.h"

int main() {
	START_PROFILE;
	
	// setup
	Textures::init();
	Game game;
	game.mainMenu();
	game.mainloop();



	END_PROFILE;
}

