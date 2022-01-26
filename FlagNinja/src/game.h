#pragma once

#include <chrono>
#include <fstream>
#include <thread>

#include "player.h"

class Timer {

	double startTime = 0.0;
	double currentTime = 0.0;
	float deltaTime = 0.0f;

	std::chrono::steady_clock::duration lastFrameTime;
	std::chrono::steady_clock::duration currentFrameTime;

	std::chrono::steady_clock::duration getTime(void* _) {
		return std::chrono::high_resolution_clock::now().time_since_epoch();
	}

	double getTime() {
		return double(std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 1000000.0;
	}

public:

	Timer() { startTime = currentTime = float(getTime()); }

	const float& getDeltaTime() { return deltaTime; };

	void update() {
		currentFrameTime = getTime(nullptr);
		deltaTime = float((currentFrameTime - lastFrameTime).count() / 1000000000.0);
		lastFrameTime = currentFrameTime;
	}

};


class Game {

	std::vector<Player> players;
	std::vector<Platform> platforms;

	sf::Vector2f cameracamera = sf::Vector2f(0.0f, 0.0f);

public:

	Timer timer;
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(1200, 800), "Flag Ninja");
	//sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(1200, 800), "Flag Ninja", sf::Style::Fullscreen);

	// constructor
	Game() {
		readMap(0);
		//window.setFramerateLimit(60);
	}

	// getters
	std::vector<Player>& getPlayers() { return players; }
	const std::vector<Platform>& getPlatforms() { return platforms; }

	void getCameraPosition() {

		// get positions and sizes
		auto player = players[0].getPosition();
		auto display = sf::Vector2f(window.getSize());
		sf::Vector2f idk = display / 1.2f;
		
		player -= idk / 2.0f;
		display -= idk;

		sf::Vector2f camera = { 0.0f, 0.0f };

		if (player.x > display.x)
			camera.x = display.x - player.x;
		else if (player.x < 0.0f)
			camera.x = -player.x;

		if (player.y > display.y)
			camera.y = display.y - player.y;
		else if (player.y < 0.0f)
			camera.y = -player.y;


		for (auto& player : players)
			player.cameraMoveBy(camera);
		for (auto& platform : platforms)
			platform.setPosition(platform.getPosition() + camera);
	}

	// read and load map file
	void readMap(int num) {
		std::string line;
		std::ifstream file("./assets/maps/" + std::to_string(num) + ".txt");

		float x = 0.0f;
		float y = 0.0f;
		const float step = 64.0f;

		while (std::getline(file, line)) {
			x = -step;

			for (auto& chr : line) {
				x += step;

				// handle each character
				if (chr == ' ') continue;
				else if (chr == 'x') platforms.push_back(Platform(x, y, "./assets/floorTile.psd"));
				//else if (chr == 'g') platforms.push_back(Platform(x, y, "./assets/grass.psd"));
				//else if (chr == 'd') platforms.push_back(Platform(x, y, "./assets/dirt.psd"));
				else if (chr == 'g') platforms.push_back(Platform(x, y, "./assets/grass_2.psd"));
				else if (chr == 'd') platforms.push_back(Platform(x, y, "./assets/grass_2.psd"));
				else if (chr == 'p') players.push_back(Player("./assets/player.psd", x + 10, y));
			}
			y += step;
		}
		file.close();
	}

	// handle player stuff - good luck reading this lol
	void handlePlayerStuff() {

		players[0].handleInput(timer.getDeltaTime());
		
		for (auto& player : players) {

			for (auto& platform : platforms)
				player.checkCollision(timer.getDeltaTime(), platform);

			player.resolveCollisions(timer.getDeltaTime());
			player.update(timer.getDeltaTime());
		}
	}

	// draw objects
	void drawObjects() {
		getCameraPosition();

		for (auto& player : players) {
			window.draw(player);
		}
		for (auto& platform : platforms) {
			window.draw(platform);
		}

		window.display();
		window.clear(sf::Color(30, 50, 240));
	}

};
