#pragma once

#include "pch.h"
#include "game.h"

namespace window {
	const char* title = "FlagNinja";
	const int frameRate = 120;

#if _DEBUG
	unsigned int width = sf::VideoMode::getDesktopMode().width / 1.5f;
	unsigned int height = sf::VideoMode::getDesktopMode().height / 1.5f;
	unsigned int style = sf::Style::Default;
#else
	unsigned int width = sf::VideoMode::getDesktopMode().width;
	unsigned int height = sf::VideoMode::getDesktopMode().height;
	unsigned int style = sf::Style::Fullscreen;
#endif
}


Game::Game() {
	font.loadFromFile("./assets/fonts/comic.ttf");
	window.setFramerateLimit(window::frameRate);
}

Game::~Game() {
	for (auto& obj : enemies) delete obj;
	for (auto& obj : bullets) delete obj;
	Textures::destroy();
}

void Game::mainMenu() {

	enum class Button : int {
		Continue, SelectLevel, Settings
	};

	sf::Texture titleNameTexture;
	sf::RectangleShape titleName;
	titleNameTexture.loadFromFile("./assets/mainMenu/titleName.psd");
	titleName.setTexture(&titleNameTexture);
	//titleName.setSize((sf::Vector2f)titleName.getTexture()->getSize());
	titleName.setSize({ window::width / 2.0f, window::height / 2.0f});
	titleName.setPosition(
		(window::width - titleName.getSize().x) / 2,
		50.0f
	);

	float width = window::width / 5.0f;
	float height = window::height / 10.0f;
	float xPos = (window::width - width) / 2.0f;
	float yPos = window::height / 2.0f;

	const char* optionNames[] = {"Continue", "Select Level", "Settings"};

	while (window.isOpen()) {

		mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		bool mouseButtonDown = false;

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			// close window button
			if (sfEvent.type == sf::Event::Closed)
				window.close();

			// close window for full screens
			if (sfEvent.type == sf::Event::KeyPressed)
				if (sfEvent.key.code == sf::Keyboard::Escape)
					window.close();

			// shooting button
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;
				
		}

		// draw title name?
		//std::cout << titleName.getSize().x << ", " << titleName.getSize().y << '\n';
		window.draw(titleName);

		sf::RectangleShape button({ width, height });
		sf::Text buttonText;
		buttonText.setFont(font);

		for (int i = 0; i < 3; i++) {
			
			// draw button background
			button.setPosition(xPos, yPos + (height + 50.0f) * i);
			
			if (sf::isPointWithinRect(mousePosition, button.getPosition(), { width, height })) {
				if (mouseButtonDown) {
					if (i == 0) {
						readMap(0);
						mainloop();
					}
					else if (i == 1) {
						// go to level selection
					}
					else if (i == 2) {
						// go to settings
					}
				}
				
				button.setFillColor(sf::Color(0xff5555ff));
			}
			else
				button.setFillColor(sf::Color(0xdd2222ff));
			window.draw(button);


			// draw text
			buttonText.setString(optionNames[i]);
			buttonText.setPosition(
				xPos + (width - buttonText.getGlobalBounds().width) / 2.0f,
				(yPos + (height + 50.0f) * i) + (height / 2.0f - buttonText.getGlobalBounds().height)
			);
			window.draw(buttonText);

		}
		

		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
}

void Game::readMap(int num) {

	// clear objects
	players.clear();
	enemies.clear();
	bullets.clear();
	platforms.clear();
	coins.clear();

	// read file
	std::string line;
	std::string filename = std::to_string(num) + ".txt";
	std::ifstream file("./assets/maps/" + filename);

	bool containsPlayer = false;
	float x = 0.0f;
	float y = 0.0f;
	const float step = 64.0f;

	// iterate though file
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

	// make sure a player has been spawned
	if (!containsPlayer) {
		std::cerr << "WARNING! - No player detected when reading file (" << filename << ")\n";  // this wont work in release mode -> no console
		players.emplace_back(0.0f, 0.0f);
	}

	// ensure nothing will move at start of game
	updateGameAttributes();
	updateGameAttributes();
}

void Game::updateGameAttributes() {
	PROFILE;
	timer.update();
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

void Game::mainloop() {
	gameEnded = false;
	while (window.isOpen() && !gameEnded) {
		PROFILE;
		updateGameAttributes();  // update timer and mouse values
		handleInput();  // handle input from the user
		handleCollisions();  // handle collisions for all collidable objects
		updateEntitys();
		updateDisplay();  // draw objects and UI to window
	}
}

void Game::resetCamera() {
	moveObjects(-overallCameraDisplacement);
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

void Game::moveObjects(const sf::Vector2f& displacement) {
	for (auto& obj : players) obj.move(displacement);
	for (auto& obj : enemies) obj->move(displacement);
	for (auto& obj : bullets) obj->move(displacement);
	for (auto& obj : platforms) obj.move(displacement);
	for (auto& obj : coins) obj.move(displacement);
	flag.move(displacement);
	overallCameraDisplacement += displacement;
}

void Game::updateDisplay() {
	PROFILE;
	drawObjects();
	drawUI();

	window.display();
	window.clear(sf::Color(30, 50, 240));
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

void Game::drawUI() {
	PROFILE;

	sf::Text scoreText("Score: " + std::to_string(score), font);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setStyle(sf::Text::Bold);
	scoreText.setPosition((float)(window::width - 200), 20.0f);
	window.draw(scoreText);
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
					gameEnded = true;
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
			gameEnded = true;
		}
	}
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
				gameEnded = true;

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



