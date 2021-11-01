
// modules
#include <iostream>
#include "SFML/Graphics.hpp"

// my files
#include "player.h"
#include "platform.h"
#include "game.h"


int main() {

	// create window
	sf::RenderWindow window(sf::VideoMode(600, 600), "Title");

	// create player object
	Player player("./assets/img.png");
	Timer timer;

	// create floor
	Platform floor(100.0f, 300.0f, "./assets/floor.png");

	// mainloop
	while (window.isOpen()) {

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			if (sfEvent.type == sf::Event::Closed)
				window.close();
		}

		// update objects
		timer.update();
		player.update(timer.deltaTime);
		player.checkCollisions(floor);

		// draw objects
		window.draw(player);
		window.draw(floor);

		window.display();
		window.clear(sf::Color(0, 0, 100));
	}

	window.close();
}

