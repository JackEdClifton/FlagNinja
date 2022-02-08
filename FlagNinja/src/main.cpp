
#include "pch.h"

// my files
#include "gun.h"
#include "platform.h"
#include "entity.h"
#include "player.h"
#include "timer.h"
#include "game.h"


int main() {
	
	// setup
	Textures::init();
	Game game;

	// mainloop
	while (game.window.isOpen()) {

		// events
		game.handleSfmlEvents();

		// update game objects
		game.updateAllObjects();
		game.drawAllObjects();

	}

}

