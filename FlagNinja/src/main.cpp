
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
	Platform floor(300.0f, 300.0f, "./assets/floorTile.psd");

	// mainloop
	while (window.isOpen()) {

		if (abs(player.getPosition().x) > 1000.0f || abs(player.getPosition().y) > 1000.0f)
			std::cout << "Position:  " << player.getPosition().x << ", " << player.getPosition().y << std::endl;

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			if (sfEvent.type == sf::Event::Closed)
				window.close();
		}

		// update objects
		timer.update();
		player.update(timer.deltaTime);
		player.olcRayVsRect(floor);

		// draw objects
		window.draw(player);
		window.draw(floor);

		window.display();
		window.clear(sf::Color(0, 0, 100));
	}

	window.close();
}

