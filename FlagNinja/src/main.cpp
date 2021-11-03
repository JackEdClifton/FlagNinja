
// modules
#include <iostream>
#include "SFML/Graphics.hpp"

// my files
#include "player.h"
#include "platform.h"
#include "game.h"


int main() {

	sf::RenderWindow window(sf::VideoMode(1200, 800), "Title");
	Game game;


	// mainloop
	while (window.isOpen()) {

		for (auto& player : game.getPlayers())
			if (abs(player.getPosition().x) > 1000.0f || abs(player.getPosition().y) > 1000.0f)
				std::cout << "Absurd Position:  " << player.getPosition().x << ", " << player.getPosition().y << std::endl;

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			if (sfEvent.type == sf::Event::Closed)
				window.close();
		}

		// update objects
		game.timer.update();

		// handle floor stuff
		for (auto& platform : game.getPlatforms()) {
			window.draw(platform);
		}

		// handle player stuff
		for (auto& player : game.getPlayers()) {
			player.update(game.timer.deltaTime);

			for (auto& platform : game.getPlatforms())
				player.checkCollision(platform);
			
			window.draw(player);
		}


		window.display();
		window.clear(sf::Color(30, 50, 240));
	}

	window.close();
}

