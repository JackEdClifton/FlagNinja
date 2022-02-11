#pragma once

#include "pch.h"
#include "game.h"

namespace window {
	const char* title = "FlagNinja";
	const int frameRate = 120;

#if _DEBUG and 0
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
	font.loadFromFile("./assets/fonts/comic.ttf");
	readMap(0);
	window.setFramerateLimit(window::frameRate);
	updateGameAttributes();
}

Game::~Game() {
	for (auto& obj : enemies) delete obj;
	for (auto& obj : bullets) delete obj;
	Textures::destroy();
}

void Game::mainMenu() {

	float width = 100.0f;
	float height = 50.0f;
	float xPos = window::width / 2.0f - width;
	float yPos = 500.0f;

	while (window.isOpen()) {
		sf::RectangleShape button({ width, height });
		button.setPosition(xPos, yPos);

		button.move(0, height + 10.0f);
		window.draw(button);

		button.move(0, height + 10.0f);
		window.draw(button);

		button.move(0, height + 10.0f);
		window.draw(button);

		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
}

void Game::mainloop() {
	while (window.isOpen()) {
		PROFILE;
		updateGameAttributes();  // update timer and mouse values
		handleInput();  // handle input from the user
		handleCollisions();  // handle collisions for all collidable objects
		updateEntitys();
		updateDisplay();  // draw objects and UI to window
	}
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
	for (auto& obj : enemies) obj->move(displacement);
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
			else if (chr == '1') enemies.push_back(new Enemy(x, y));
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
	PROFILE;
	adjustCamera();

	// platforms
	for (auto& platform : platforms)
		if (isSpriteInWindow(platform, window))
			window.draw(platform);

	// players
	for (auto& player : players) {
		window.draw(player);
		window.draw(player.gun);
		player.drawHealthBar(&window);
	}

	// enemies
	for (auto& enemy : enemies) {
		window.draw(*enemy);
		window.draw(enemy->gun);
		enemy->drawHealthBar(&window);
	}


	for (auto& obj : bullets) window.draw(*obj);
	for (auto& obj : coins) window.draw(obj);

	window.draw(flag);
}

void Game::drawUI() {
	PROFILE;

	sf::Text scoreText("Score: " + std::to_string(score), font);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setPosition((float)(window::width - 200), 20.0f);
	window.draw(scoreText);
}

void Game::destroyBullet(int index) {
#if _DEBUG
	if (index >= bullets.size()) {
		std::cerr << "WARNING! - Bullet index out of range!\n";
		return;
	}
#endif
	bullets[index] = bullets[bullets.size() - 1];
	bullets.pop_back();
}

void Game::destroyEnemy(int index) {
#if _DEBUG
	if (index >= enemies.size()) {
		std::cerr << "WARNING! - Enemy index out of range!\n";
		return;
	}
#endif
	enemies[index] = enemies[enemies.size() - 1];
	enemies.pop_back();
}

void Game::updateDisplay() {
	PROFILE;
	drawObjects();
	drawUI();

	window.display();
	window.clear(sf::Color(30, 50, 240));
}

void Game::handleCollisions() {
	PROFILE;

	// players
	for (auto& player : players) {
		for (auto& platform : platforms)
			player.checkCollision(deltaTime, platform);
		player.resolveCollisions(deltaTime);
	}

	// enemys
	for (auto& enemy : enemies) {
		for (auto& platform : platforms)
			enemy->checkCollision(deltaTime, platform);
		enemy->resolveCollisions(deltaTime);
	}

	// bullets
	for (unsigned int i = 0; i < bullets.size(); i++) {
		bullets[i]->update(deltaTime);

		// timeout
		if (bullets[i]->bulletTimeout(deltaTime)) {
			destroyBullet(i);
			goto exitBulletLoop;
		}

		// platforms
		for (auto& platform : platforms) {
			if (isColliding(*bullets[i], platform)) {
				destroyBullet(i);
				goto exitBulletLoop;
			}
		}

		// player
		for (auto& player : players) {
			if (isColliding(*bullets[i], player)) {
				if (player.hit(2.0f)) {
					window.close();
				}
				destroyBullet(i);
				goto exitBulletLoop;
			}
		}

		// enemy
		for (auto& enemy : enemies) {
			if (isColliding(*bullets[i], *enemy)) {
				if (enemy->hit(20.0f)) {
					destroyEnemy(i);
					score += 100;
				}
				destroyBullet(i);
				goto exitBulletLoop;
			}
		}
	}
exitBulletLoop:

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
	PROFILE;
	timer.update();
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

void Game::updateEntitys() {
	PROFILE;
	for (auto& obj : players) obj.update(deltaTime, mousePosition);
	for (auto& obj : enemies) obj->update(deltaTime, players, platforms, bullets);
	for (auto& obj : coins) obj.update(deltaTime);
	flag.update(deltaTime);
}

void Game::handleInput() {
	PROFILE;

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
			players[0].shoot(bullets);
	}

	// user input for first player
	if (GetAsyncKeyState('A')) players[0].moveLeft();
	if (GetAsyncKeyState('D')) players[0].moveRight();
	if (GetAsyncKeyState('S')) players[0].moveDown();
	if ((GetAsyncKeyState('W') || GetAsyncKeyState(' '))) players[0].jump();

	// temp
	for (auto& enemy : enemies) {
		enemy->moveRight();
		enemy->jump();
	}

	// reset position for debugging
	if (GetAsyncKeyState('R')) {
		resetCamera();
		players[0].setPosition(250.0f, 250.0f);
	}
}



