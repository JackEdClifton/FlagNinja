#pragma once

#include "pch.h"
#include "game.h"

namespace window {
	const char* title = "FlagNinja";
	const int frameRate = 0;

#if _DEBUG
	unsigned int width = sf::VideoMode::getDesktopMode().width / 2;
	unsigned int height = sf::VideoMode::getDesktopMode().height / 2;
	unsigned int style = sf::Style::Default;
#else
	unsigned int width = sf::VideoMode::getDesktopMode().width;
	unsigned int height = sf::VideoMode::getDesktopMode().height;
	unsigned int style = sf::Style::Fullscreen;
#endif
}


Game::Game() {
	readMap(0);
	window.setFramerateLimit(window::frameRate);
	updateGameAttributes();
}

Game::~Game() {
	for (auto& obj : bullets) delete obj;
	Textures::destroy();
}

void Game::adjustCamera() {

	// get positions and sizes
	sf::Vector2f player = players[0].getPosition();
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

	moveObjects(camera);
}

void Game::resetCamera() {
	moveObjects(-overallCameraDisplacement);
}

void Game::moveObjects(const sf::Vector2f& displacement) {
	for (auto& obj : players) obj.move(displacement);
	for (auto& obj : enemies) obj.move(displacement);
	for (auto& obj : bullets) obj->move(displacement);
	for (auto& obj : platforms) obj.move(displacement);
	for (auto& obj : coins) obj.move(displacement);
	flag.move(displacement);
	overallCameraDisplacement += displacement;
}

void Game::readMap(int num) {

	std::string line;
	std::string filename = std::to_string(num) + ".txt";
	std::ifstream file("./assets/maps/" + filename);

	bool containsPlayer = false;

	float x = 0.0f;
	float y = 0.0f;
	const float step = 64.0f;

	while (std::getline(file, line)) {
		x = -step;

		for (auto& chr : line) {
			x += step;

			// handle each character
			if (chr == ' ') continue;
			else if (chr == 'g') platforms.emplace_back(x, y, Textures::Grass);
			else if (chr == 'd') platforms.emplace_back(x, y, Textures::Dirt);
			else if (chr == 'f') flag.setPosition(x, y);			
			else if (chr == 'c') coins.emplace_back(x, y);
			else if (chr == 'p') {
				players.emplace_back(x, y);
				containsPlayer = true;
			}
			else if (chr == '1') enemies.emplace_back(x, y);
		}
		y += step;
	}
	file.close();

	if (!containsPlayer) {
		std::cerr << "WARNING! - No player detected when reading file (" << filename << ")\n";  // this wont work in release mode -> no console
		players.emplace_back(0.0f, 0.0f);
	}
}

void Game::drawObjects() {

	adjustCamera();

	for (auto& platform : platforms)
		if (isSpriteInWindow(platform, window))
			window.draw(platform);

	for (auto& player : players) {
		window.draw(player);
		window.draw(player.gun);
	}

	for (auto& obj : enemies) window.draw(obj);
	for (auto& obj : bullets) window.draw(*obj);
	for (auto& obj : coins) window.draw(obj);

	window.draw(flag);
}

void Game::drawUI() {
	sf::Font font;
	font.loadFromFile("./assets/fonts/comic.ttf");

	sf::Text scoreText("Score: " + std::to_string(score), font);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setPosition((float)(window::width - 200), 20.0f);
	window.draw(scoreText);

}

void Game::updateDisplay() {
	drawObjects();
	drawUI();

	window.display();
	window.clear(sf::Color(30, 50, 240));
}

void Game::handleCollisions() {

	// players
	for (auto& player : players) {		
		for (auto& platform : platforms)
			player.checkCollision(deltaTime, platform);
		player.resolveCollisions(deltaTime);
	}

	// enemys
	for (auto& enemy : enemies) {
		for (auto& platform : platforms)
			enemy.checkCollision(deltaTime, platform);
		enemy.resolveCollisions(deltaTime);
	}

	// bullets
	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i]->update(deltaTime);

		// check if bullet has exited for too long
		if (bullets[i]->bulletTimeout(deltaTime))
			goto destroyBullet;

		// check if bullet has collided with anything
		for (auto& platform : platforms) {
			if (isColliding(*bullets[i], platform))
				goto destroyBullet;
		}

		continue;
	destroyBullet:
		bullets[i] = bullets[bullets.size() - 1];
		bullets.pop_back();
	}

	// coins
	for (unsigned int i = 0; i < coins.size(); i++) {
		for (auto& player : players) {
			if (isColliding(coins[i], player)) {
				score += 20;
				coins[i] = coins[coins.size() - 1];
				coins.pop_back();
			}
		}
	}

	// flag
	for (auto& player : players) {
		if (isColliding(flag, player)) {
			score += 200;
			window.close();
		}
	}
}

void Game::updateGameAttributes() {
	timer.update();
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

void Game::updateEntitys() {
	for (auto& obj : players) obj.update(deltaTime, mousePosition);
	for (auto& obj : enemies) obj.update(deltaTime);
	for (auto& obj : coins) obj.update(deltaTime);
	flag.update(deltaTime);
}

void Game::handleInput() {

	// sfml events
	sf::Event sfEvent;
	while (window.pollEvent(sfEvent)) {

		// close window button
		if (sfEvent.type == sf::Event::Closed)
			window.close();

		// close game -> pause menu
		if (sfEvent.type == sf::Event::KeyPressed)
			if (sfEvent.key.code == sf::Keyboard::Escape)
				window.close();

		// shooting button
		if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
			bullets.push_back(new Bullet(players[0].gun.getPosition(), players[0].gun.getUnitVector()));
	}

	// other events
	// user input for first player
	if (GetAsyncKeyState('A')) players[0].moveLeft();
	if (GetAsyncKeyState('D')) players[0].moveRight();
	if (GetAsyncKeyState('S')) players[0].moveDown();
	if ((GetAsyncKeyState('W') || GetAsyncKeyState(' '))) players[0].jump();
	
	if (GetAsyncKeyState('R')) {  // reset position for debugging
		resetCamera();
		players[0].setPosition(250.0f, 250.0f);
	}
}



