
// modules
#include <iostream>
#include "SFML/Graphics.hpp"

// my files
#include "player.h"
#include "platform.h"
#include "game.h"



int main() {


	sf::RenderWindow window(sf::VideoMode(1200, 800), "Flag Ninja");
	std::cout << "Flag Ninja!\n";
	window.setFramerateLimit(60);

	Game game;
	std::cout << std::fixed;

	auto x = game.getPlayers()[0].getPosition();
	std::cout << x.x << " " << x.y << std::endl;

	// mainloop
	while (window.isOpen()) {

		// events
		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {
			if (sfEvent.type == sf::Event::Closed)
				window.close();
		}

		// update game objects
		game.timer.update(nullptr);

		// handle floor stuff
		for (auto& platform : game.getPlatforms()) {
			window.draw(platform);
		}

		// handle player stuff
		for (auto& player : game.getPlayers()) {
			player.handleInput(game.timer.getDeltaTime());
			player.updatePhysics(game.timer.getDeltaTime());
			window.draw(player);
			for (auto& platform : game.getPlatforms())
				player.checkCollision(game.timer.getDeltaTime(), platform);
			player.resolveCollisions(game.timer.getDeltaTime());
			player.update(game.timer.getDeltaTime());

			std::cout << (int)player.getPosition().x << ", " << (int)player.getPosition().y << std::endl;
		}


		window.display();
		window.clear(sf::Color(30, 50, 240));
	}

	window.close();
}

