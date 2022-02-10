#pragma once

#include "timer.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "coin.h"
#include "flag.h"
#include "staticEntity.h"

namespace window {
	extern const char* title;
	extern unsigned int width;
	extern unsigned int height;
	extern unsigned int style;
}

class Game {
	sf::Vector2f overallCameraDisplacement = { 0.0f, 0.0f };
	sf::Vector2f mousePosition;

	int score = 0;

	sf::Font font;

	void readMap(int num);  // read and load map file
	void adjustCamera();  // move all drawable objects to keep player within window
	void resetCamera();
	void moveObjects(const sf::Vector2f& displacement);
	void drawObjects();  // draw objects
	void drawUI();  // draw user interface

public:

	Game();
	~Game();

	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(window::width, window::height), window::title, window::style);
	Timer timer;
	const float& deltaTime = timer.getDeltaTime();

	// drawable objects
	std::vector<Player> players;
	std::vector<Enemy> enemies;
	std::vector<Bullet*> bullets;
	std::vector<StaticEntity> platforms;
	std::vector<Coin> coins;
	Flag flag;

	// abstract game functions
	void updateDisplay();
	void updateGameAttributes(); // update timer and mouse values
	void handleCollisions();  // handle collisions for all collidable objects
	void updateEntitys(); // apply velocity
	void handleInput();  // and window update position and attributes of objects
};
