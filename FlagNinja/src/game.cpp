#pragma once

#include <chrono>
#include <fstream>
#include <thread>
#include <vector>

#include "SFML/Graphics.hpp"

#include "player.h"
#include "platform.h"
#include "entity.h"
#include "gun.h"
#include "timer.h"

#include "game.h"


// constructor
Game::Game() {
	readMap(0);
}

Game::~Game() {
	for (auto& obj : players) delete obj;
	for (auto& obj : platforms) delete obj;
	for (auto& obj : bullets) delete obj;
}

// getters
std::vector<Player*>& Game::getPlayers() { return players; }
const std::vector<Platform*>& Game::getPlatforms() { return platforms; }

void Game::adjustCameraPosition() {

	// get positions and sizes
	auto player = (*players[0]).getPosition();
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
		player->cameraMoveBy(camera);
	for (auto& platform : platforms)
		platform->move(camera);
	for (auto& bullet : bullets)
		if (bullet) bullet->move(camera);
}

// read and load map file
void Game::readMap(int num) {
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
			else if (chr == 'x') platforms.push_back(new Platform(x, y, "floorTile"));
			else if (chr == 'g') platforms.push_back(new Platform(x, y, "grass_2"));
			else if (chr == 'd') platforms.push_back(new Platform(x, y, "grass_2"));
			else if (chr == 'p') players.push_back(new Player("player", x + 10, y));
			y = y + 1 - 1;
		}
		y += step;
	}
	file.close();
}

// handle player movement
void Game::handlePlayers() {
	for (auto& player : players) {

		for (auto& platform : platforms)
			player->checkCollision(timer.getDeltaTime(), *platform);

		player->resolveCollisions(timer.getDeltaTime());
		player->updateMovement(timer.getDeltaTime());
		player->updateAnimation(timer.getDeltaTime());
	}
}

// draw objects
void Game::drawObjects() {
	adjustCameraPosition();

	for (auto& platform : platforms)
		window.draw(*platform);

	for (auto& player : players) {
		window.draw(*player);
		window.draw(player->getGun());
	}

	for (auto& bullet : bullets)
		window.draw(*bullet);

	window.display();
	window.clear(sf::Color(30, 50, 240));
}

// update values
void Game::update() {
	timer.update();
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	players[0]->handleInput(timer.getDeltaTime(), mousePosition);
	int index = 0;
	for (auto& bullet : bullets) {
		bullet->update(timer.getDeltaTime());
		if (bullet->bulletTimeout(timer.getDeltaTime()) && false) {
			bullets[index] = bullets[bullets.size() - 1];
			bullets.pop_back();
		}
		index++;
	}
}

// handle sfml specific events
void Game::handleSfmlEvents() {
	sf::Event sfEvent;
	while (window.pollEvent(sfEvent)) {

		// close window button
		if (sfEvent.type == sf::Event::Closed)
			window.close();

		// shooting button
		if (sfEvent.type == sf::Event::MouseButtonPressed)
			bullets.push_back(new Bullet(players[0]->getPosition(), players[0]->getGun().getUnitVector(), players[0]->getGun().getRotation()));
	}
}