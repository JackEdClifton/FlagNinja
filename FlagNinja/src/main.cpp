
// modules
#include <chrono>
#include <fstream>
#include <thread>

#include "SFML/Graphics.hpp"

// my files
#include "sfmlExtension.h"
#include "textuers.h"
#include "gun.h"
#include "platform.h"
#include "entity.h"
#include "player.h"
#include "timer.h"
#include "game.h"

int main() {
	
	// setup
	Textures::init();

	start:
	Game game;

	// mainloop
	while (game.window.isOpen()) {

		// events
		game.handleSfmlEvents();
		std::cout << (game.getPlayers()[0]->getPosition().x) << " " << (game.getPlayers()[0]->getPosition().y) << std::endl;
		if (GetAsyncKeyState('Q'))
			game.getPlayers()[0]->setP();

		// update game objects
		game.update();
		game.handlePlayers();
		game.drawObjects();

	}

	goto start;
}

