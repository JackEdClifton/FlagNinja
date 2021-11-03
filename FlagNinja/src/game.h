#pragma once

#include <chrono>
#include <fstream>

class Timer {

	float startTime = 0.0f;
	float currentTime = 0.0f;

	static float getTime() {
		return float(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 100000000.0f;
	}

public:
	float deltaTime = 0.0f;

	Timer() {
		startTime = getTime();
		currentTime = getTime();
	}


	void update() {
		currentTime = getTime();
		deltaTime = currentTime - startTime;
		startTime = currentTime;
	}

};


#include <iostream>
class Game {

	std::vector<Player> players;
	std::vector<Platform> platforms;

public:

	Timer timer;

	Game() {
		readMap(0);
	}

	std::vector<Player>& getPlayers() { return players; }
	const std::vector<Platform>& getPlatforms() { return platforms; }

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
				else if (chr == 'p') { players.push_back(Player("./assets/player.psd", x + 10, y)); std::cout << x+10 << " " << y << std::endl; }

			}
			y += step;
		}
		file.close();
	}

};
