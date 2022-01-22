#pragma once

#include <chrono>
#include <fstream>
#include <thread>

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
		currentTime = getTime();
		deltaTime = float(currentTime - startTime);
		startTime = currentTime;
	}

	void update(void*) {
		currentFrameTime = getTime(nullptr);
		deltaTime = float((currentFrameTime - lastFrameTime).count() / 1000000000.0);
		lastFrameTime = currentFrameTime;
	}

};


class Game {

	std::vector<Player> players;
	std::vector<Platform> platforms;

	sf::Vector2f cameraDisplacement = sf::Vector2f(0.0f, 0.0f);

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
				else if (chr == 'p') players.push_back(Player("./assets/player.psd", x + 10, y));
			}
			y += step;
		}
		file.close();
	}

};
