
// modules
#include <iostream>
#include "SFML/Graphics.hpp"

// my files
#include "game.h"



int main() {
	
	// setup
	Game game;

	// mainloop
	while (game.window.isOpen()) {

		// events
		sf::Event sfEvent;
		while (game.window.pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed)
				game.window.close();
		}

		// update game objects
		game.timer.update();
		game.handlePlayerStuff();
		game.drawObjects();

	}
}

