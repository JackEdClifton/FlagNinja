#pragma once


#include "pch.h"

#include "platform.h"
#include "gun.h"
#include "entity.h"
#include "player.h"
#include "timer.h"

#include "game.h"


// constructor
Game::Game() {
	readMap(0);
}

// destructor
Game::~Game() {
	for (auto& obj : players) delete obj;
	for (auto& obj : platforms) delete obj;
	for (auto& obj : bullets) delete obj;
}

// getters
std::vector<Player*>& Game::getPlayers() {
	return players;
}
const std::vector<Platform*>& Game::getPlatforms() {
	return platforms;
}

// move all objects so player fits in window
void Game::adjustCameraPosition() {

	// get positions and sizes
	sf::Vector2f player = players[0]->getPosition();
	sf::Vector2f display = sf::Vector2f(window.getSize());
	sf::Vector2f safezone = display / 1.2f;

	player -= safezone / 2.0f;
	display -= safezone;

	sf::Vector2f camera = { 0.0f, 0.0f };

	if (player.x > display.x)
		camera.x = display.x - player.x;
	else if (player.x < 0.0f)
		camera.x = -player.x;

	if (player.y > display.y)
		camera.y = display.y - player.y;
	else if (player.y < 0.0f)
		camera.y = -player.y;

	overallCameraDisplacement += camera;

	for (auto& player : players)
		player->cameraMoveBy(camera);
	for (auto& platform : platforms)
		platform->move(camera);
	for (auto& bullet : bullets)
		if (bullet) bullet->move(camera);
}

// load a map from a file
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
			else if (chr == 'x') platforms.push_back(new Platform(x, y, Textures::Grass));
			else if (chr == 'g') platforms.push_back(new Platform(x, y, Textures::Grass));
			else if (chr == 'd') platforms.push_back(new Platform(x, y, Textures::Dirt));
			else if (chr == 'p') players.push_back(new Player(x + 10, y, "player"));
		}
		y += step;
	}
	file.close();
}

// draw objects
void Game::drawAllObjects() {
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

// update object values
void Game::updateAllObjects() {

	// game
	timer.update();
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));

	// players
	players[0]->handleInput(timer.getDeltaTime(), mousePosition);
	for (auto& player : players) {
		
		for (auto& platform : platforms)
			player->checkCollision(timer.getDeltaTime(), *platform);

		player->resolveCollisions(timer.getDeltaTime());
		player->updateMovement(timer.getDeltaTime());
		player->updateAnimation(timer.getDeltaTime());
	}

	// bullets
	for (unsigned int i = 0; i < bullets.size(); i++) {

		bullets[i]->update(timer.getDeltaTime());

		// check if bullet has exited for too long
		if (bullets[i]->bulletTimeout(timer.getDeltaTime()))
			goto destroyBullet;

		// check if bullet has collided with anything
		for (auto& platform : platforms) {
			if (bullets[i]->isColliding(*platform))
				goto destroyBullet;
		}

		continue;
	destroyBullet:
		bullets[i] = bullets[bullets.size() - 1];
		bullets.pop_back();
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
		if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
			bullets.push_back(new Bullet(players[0]->getPosition(), players[0]->getGun().getUnitVector(), players[0]->getGun().getRotation()));
	}
}