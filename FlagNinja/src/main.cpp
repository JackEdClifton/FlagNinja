
#include "pch.h"
#include "game.h"

int main() {
	
	// setup
	Textures::init();
	Game game;

	START_PROFILE;

	// mainloop
	while (game.window.isOpen()) {
		PROFILE;
		game.updateDisplay();  // draw objects and UI to window
		game.updateGameAttributes();  // update timer and mouse values
		game.handleInput();  // handle input from the user
		game.handleCollisions();  // handle collisions for all collidable objects
		game.updateEntitys();
	}

	END_PROFILE;
}

