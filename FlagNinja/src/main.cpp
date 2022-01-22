
// modules
#include <iostream>
#include "SFML/Graphics.hpp"

// my files
#include "player.h"
#include "platform.h"
#include "game.h"
#include "entity.h"



int main() {
	
	// setup
	Game game;
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Flag Ninja");
	window.setFramerateLimit(60);


	// mainloop
	while (window.isOpen()) {

		// events
		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed)
				window.close();
		}

		// update game objects
		game.timer.update();

		// handle floor stuff
		for (auto& platform : game.getPlatforms()) {
			window.draw(platform);
		}

		// handle player stuff - good luck reading this lol
		for (auto& player : game.getPlayers()) {
			player.handleInput(game.timer.getDeltaTime());
			window.draw(player);
			for (auto& platform : game.getPlatforms())
				player.checkCollision(game.timer.getDeltaTime(), platform);
			player.resolveCollisions(game.timer.getDeltaTime());
			player.update(game.timer.getDeltaTime());
		}

		// update window
		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
}

