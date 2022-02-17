
#include "pch.h"
#include "game.h"

// container for window attributes
namespace window {
	const char* title = "Flag Ninja";
	const int frameRate = 120;

#if _DEBUG
	unsigned int width = (int)(sf::VideoMode::getDesktopMode().width / 1.5f);
	unsigned int height = (int)(sf::VideoMode::getDesktopMode().height / 1.5f);
	unsigned int style = sf::Style::Default;
#else
	unsigned int width = (int)(sf::VideoMode::getDesktopMode().width);
	unsigned int height = (int)(sf::VideoMode::getDesktopMode().height);
	unsigned int style = sf::Style::Fullscreen;
#endif
}

// container for game settings
namespace settings {
	bool isVsyncEnabled = true;
	bool hardMode = false;
	bool playMusic = false;

	bool* options[] = { &isVsyncEnabled, &hardMode, &playMusic };
}

// game constructor
Game::Game() {

	// load font
	font.loadFromFile("./assets/fonts/comic.ttf");

	// load background image
	background.setTexture(Textures::Background);
	background.setSize((sf::Vector2f)Textures::Background->getSize());
	background.setPosition(-500.0f, -500.0f);
	
	// load background audio
	backgroundMusicBuffer.loadFromFile("./assets/audio/project 1.wav");
	backgroundMusic.setBuffer(backgroundMusicBuffer);
	backgroundMusic.setVolume(20.0f);
	backgroundMusic.setLoop(true);
	
	// apply settings if necacerry
	window.setVerticalSyncEnabled(settings::isVsyncEnabled);
	if (settings::playMusic) backgroundMusic.play();
}

// game deconstructor
Game::~Game() {
	for (auto& obj : enemies) delete obj;
	for (auto& obj : bullets) delete obj;
	Textures::destroy();
}

// game loop for main menu
void Game::mainMenu() {

	// setup logo objects
	sf::Texture logoTexture;
	sf::RectangleShape logo;
	logoTexture.loadFromFile("./assets/mainMenu/logo.psd");
	logo.setTexture(&logoTexture);
	logo.setSize({ window::width / 2.0f, window::height / 2.0f });
	logo.setPosition(
		(window::width - logo.getSize().x) / 2,
		20.0f
	);

	// setup button objects
	sf::RectangleShape button;
	sf::Text buttonText;
	buttonText.setFont(font);
	buttonText.setFillColor(sf::Color::Black);

	// setup position and size for options
	float width = window::width / 5.0f;
	float height = window::height / 10.0f;
	float xPos = (window::width - width) / 2.0f;
	float yPos = window::height / 2.0f;

	const char* optionNames[] = { "Continue", "Select Level", "Settings" };

	// game loop
	while (window.isOpen()) {

		// get mouse position and button state
		mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		bool mouseButtonDown = false;

		// sfml events
		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			// close window
			if (sfEvent.type == sf::Event::KeyPressed)
				if (sfEvent.key.code == sf::Keyboard::Escape)
					return;

			// check if shooting button has been clicked
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;

		}

		// draw boxes and text for options
		for (int i = 0; i < 3; i++) {

			// set default button variables
			button.setFillColor(sf::Color(0xdd2222ff));
			button.setPosition(xPos, yPos + (height + 50.0f) * i);
			button.setSize({ width, height });

			if (sf::isPointWithinRect(mousePosition, button.getPosition(), { width, height })) {
				
				// set mouse hovering button variables
				button.setFillColor(sf::Color(0xff2222ff));
				button.setPosition(xPos - 10.0f, yPos - 10.0f + (height + 50.0f) * i);
				button.setSize({ width + 20.0f, height + 20.0f });

				// if user has clicked perform an action
				if (mouseButtonDown) {
					if (i == 0)
						playMap("1.txt");
					else if (i == 1)
						levelSelection();
					else if (i == 2)
						settings();
				}
			}

			window.draw(button);

			// draw text
			buttonText.setString(optionNames[i]);
			buttonText.setPosition(
				xPos + (width - buttonText.getGlobalBounds().width) / 2.0f,
				(yPos + (height + 50.0f) * i) + (buttonText.getGlobalBounds().height) / 4.0f
			);
			window.draw(buttonText);

		}

		// draw logo
		window.draw(logo);

		// update display
		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
}

// game loop to play a level
void Game::playMap(const std::string& map) {

	// load a map
	readMap(map);
	
	// game loop for main game
	gameEnded = false;
	totalTimer = 0.0f;
	while (window.isOpen() && !gameEnded) {
		updateGameAttributes();  // update timer and mouse values
		handleInput();  // handle input from the user
		handleCollisions();  // handle collisions for all collidable objects
		updateDisplay();  // draw objects and UI to window
		updateEntitys();  // call update method for game objects
	}
}

// game loop to select a level
void Game::levelSelection() {

	sf::Texture logoTexture;
	sf::RectangleShape logo;
	logoTexture.loadFromFile("./assets/mainMenu/logo.psd");
	logo.setTexture(&logoTexture);
	logo.setSize({ window::width / 4.0f, window::height / 4.0f });
	logo.setPosition(50.0f, 50.0f);

	// not needed yet but we dont need this inside the game loop
	// sizes and positons for level buttons
	const float width = window::width / 6.0f;
	const float height = window::height / 6.0f;
	const float xPos = (window::width - width) / 4.0f;
	const float yPos = 50.0f;
	float scroll = 0.0f;

	// mainloop for level selection
	while (window.isOpen()) {

		mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		bool mouseButtonDown = false;

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			// close window for full screens
			if (sfEvent.type == sf::Event::KeyPressed)
				if (sfEvent.key.code == sf::Keyboard::Escape)
					return;

			// scroll through levels
			if (sfEvent.type == sf::Event::MouseWheelMoved) {
				scroll -= sfEvent.mouseWheel.delta * 10.0f;

				const float maxScroll = 0.0f;

				if (scroll < 0.0f) scroll = 0.0f;
				if (scroll > maxScroll) scroll = maxScroll;
			}

			// shooting button
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;

		}

		// draw title name
		window.draw(logo);


		// button to draw options
		sf::RectangleShape button({ width, height });
		sf::Text buttonText;
		buttonText.setFont(font);
		buttonText.setFillColor(sf::Color::Black);

		int row = 0;
		int column = 1;

		for (auto& file : std::filesystem::directory_iterator("./assets/maps")) {
			std::string filename = file.path().filename().string();

			// draw background
			button.setPosition(xPos + (width + 20.0f) * column, yPos + (height + 20.0f) * row - scroll);

			if (sf::isPointWithinRect(mousePosition, button.getPosition(), button.getSize())) {
				button.setFillColor(sf::Color(0xff2222ff));

				if (mouseButtonDown) {
					playMap(filename);
					return;
				}
			}
			else
				button.setFillColor(sf::Color(0xdd2222ff));

			window.draw(button);


			// draw text
			buttonText.setString(filename.substr(0, filename.size() - 4));
			const sf::Vector2f textSize = { buttonText.getGlobalBounds().width , buttonText.getGlobalBounds().height };
			buttonText.setPosition(button.getPosition() + (button.getSize() - textSize) / 2.0f);

			window.draw(buttonText);

			// update column and row values
			if (column % 3) {
				column += 1;
			}
			else {
				row += 1;
				column = 1;
			}
		}
		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
}

// game loop to modify game settings
void Game::settings() {
	sf::Texture logoTexture;
	sf::RectangleShape logo;
	logoTexture.loadFromFile("./assets/mainMenu/logo.psd");
	logo.setTexture(&logoTexture);
	logo.setSize({ window::width / 4.0f, window::height / 4.0f });
	logo.setPosition(50.0f, 50.0f);

	sf::RectangleShape button;
	sf::Text buttonText;
	buttonText.setFont(font);
	buttonText.setFillColor(sf::Color::Black);

	const char* optionNames[] = { "V Sync", "Hard Mode", "Background Music" };

	float scroll = 0.0f;

	// mainloop for level selection
	while (window.isOpen()) {

		mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		bool mouseButtonDown = false;

		sf::Event sfEvent;
		while (window.pollEvent(sfEvent)) {

			// close window for full screens
			if (sfEvent.type == sf::Event::KeyPressed)
				if (sfEvent.key.code == sf::Keyboard::Escape)
					return;

			// scroll through levels
			if (sfEvent.type == sf::Event::MouseWheelMoved) {
				scroll -= sfEvent.mouseWheel.delta * 10.0f;

				const float maxScroll = 0.0f;

				if (scroll < 0.0f) scroll = 0.0f;
				if (scroll > maxScroll) scroll = maxScroll;
			}

			// shooting button
			if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
				mouseButtonDown = true;

		}

		const float width = window::width / 5.0f;
		const float height = window::height / 10.0f;
		const float xPos = (window::width - width) / 2.0f;
		const float yPos = window::height / 5.0f;

		// draw options
		for (int i = 0; i < 3; i++) {

			// set colour
			if (*settings::options[i])
				button.setFillColor(sf::Color(0x22ff22ff));
			else
				button.setFillColor(sf::Color(0xdd2222ff));


			// draw button background
			button.setPosition(xPos - 10.0f, yPos - 10.0f + (height + 50.0f) * i);
			button.setSize({ width + 20.0f, height + 20.0f });
			if (sf::isPointWithinRect(mousePosition, button.getPosition(), { width, height })) {
				if (mouseButtonDown) {
					*settings::options[i] = !*settings::options[i];
					if (i == 0)
						window.setVerticalSyncEnabled(settings::isVsyncEnabled);
					
					if (i == 2) {
						if (settings::playMusic) backgroundMusic.play();
						else backgroundMusic.pause();
					}

				}

			}
			else {
				button.setPosition(xPos, yPos + (height + 50.0f) * i);
				button.setSize({ width, height });
			}
			window.draw(button);


			// draw text
			buttonText.setString(optionNames[i]);
			buttonText.setPosition(
				xPos + (width - buttonText.getGlobalBounds().width) / 2.0f,
				(yPos + (height + 50.0f) * i) + (buttonText.getGlobalBounds().height) / 4.0f
			);
			window.draw(buttonText);

		}


		window.draw(logo);

		window.display();
		window.clear(sf::Color(30, 50, 240));
	}
}

// construct objects for a map into memory
void Game::readMap(const std::string& filename) {

	// clear objects
	players.clear();
	enemies.clear();
	bullets.clear();
	platforms.clear();
	coins.clear();

	// read file
	std::string line;
	std::ifstream file("./assets/maps/" + filename);

	bool containsPlayer = false;
	float x = 0.0f;
	float y = 0.0f;
	const float step = 64.0f;

	totalCoins = collectedCoins = totalEnemies = killedEnemies = 0;

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
			else if (chr == 'c') {
				coins.emplace_back(x, y);
				totalCoins += 1;
			}
			else if (chr == 'p') {
				players.emplace_back(x, y);
				containsPlayer = true;
			}
			else if (chr == '1') {
				enemies.push_back(new Enemy(x, y));
				totalEnemies += 1;
			}
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


	for (auto& player : players) player.moveRight();
	for (auto& enemy : enemies) enemy->moveRight();
}

// update timer and mouse position
void Game::updateGameAttributes() {
	timer.update();
	totalTimer += deltaTime;
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

// move all drawable objects by a fixed amount
void Game::moveObjects(const sf::Vector2f& displacement) {
	for (auto& obj : players) obj.move(displacement);
	for (auto& obj : enemies) obj->move(displacement);
	for (auto& obj : bullets) obj->move(displacement);
	for (auto& obj : platforms) obj.move(displacement);
	for (auto& obj : coins) obj.move(displacement);
	flag.move(displacement);
	background.move(displacement* sf::Vector2f(0.0f, 0.2f));  // moving the background, but only a little
	overallCameraDisplacement += displacement;
}

// move all objects so player fits in frame
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

	camera.x = (float)((int)camera.x);
	camera.y = (float)((int)camera.y);

	moveObjects(camera);
}

// reset camera to default position - debug
void Game::resetCamera() {
	moveObjects(-overallCameraDisplacement);
}

// destroy a bullet object
void Game::destroyBullet(unsigned int index) {
	bullets[index] = bullets[bullets.size() - 1];
	bullets.pop_back();
}

// destroy an enemy object
void Game::destroyEnemy(unsigned int index) {
	enemies[index] = enemies[enemies.size() - 1];
	enemies.pop_back();
	killedEnemies += 1;
}

// draw objects and UI
void Game::updateDisplay() {
	drawObjects();
	drawUI();

	window.display();
	window.clear(sf::Color(30, 50, 240));
	//window.draw(background);
}

// draw objects to the screen
void Game::drawObjects() {
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

// draw the UI (text)
void Game::drawUI() {

	// setup text object
	sf::Text text;
	text.setFont(font);
	text.setFillColor(sf::Color::White);
	text.setStyle(sf::Text::Bold);

	// fps counter
	text.setString("fps: " + std::to_string(int(1.0f / deltaTime)));
	text.setPosition(50.0f, 20.0f);
	window.draw(text);

	// score counter
	text.setString("Score: " + std::to_string(score));
	text.setPosition(200.0f, 20.0f);
	window.draw(text);

	// coins counter
	text.setString("Coins: " + std::to_string(collectedCoins) + "/" + std::to_string(totalCoins));
	text.setPosition(450.0f, 20.0f);
	window.draw(text);

	// enemy counter
	text.setString("Enemies: " + std::to_string(killedEnemies) + "/" + std::to_string(totalEnemies));
	text.setPosition(700.0f, 20.0f);
	window.draw(text);

	// game timer
	text.setString("Timer: " + std::to_string(int(totalTimer)) + "." + std::to_string(int(totalTimer*10.0f) % 10));
	text.setPosition(950.0f, 20.0f);
	window.draw(text);
}

// find collisions and handle them apropriately
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
				if (player.hit(0.2f + 1.0f * settings::hardMode)) {
					gameEnded = true;
				}
				destroyBullet(i);
				goto exitBulletLoop;
			}
		}

		// enemy
		for (unsigned int j = 0; j < enemies.size(); j++) {
			if (isColliding(*bullets[i], *enemies[j])) {
				if (enemies[j]->hit(20.0f)) {
					destroyEnemy(j);
					score += 100;
				}
				destroyBullet(i);
				goto exitBulletLoop;
			}
		}
	}
exitBulletLoop:

	// coins
	for (unsigned int j = 0; j < players.size(); j++) {
		for (unsigned int i = 0; i < coins.size(); i++) {
			if (isColliding(coins[i], players[j])) {
				score += 20;
				coins[i] = coins[coins.size() - 1];
				coins.pop_back();
				collectedCoins += 1;
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

// call update method for game objects
void Game::updateEntitys() {
	for (auto& obj : players) obj.update(deltaTime, mousePosition);
	for (auto& obj : enemies) obj->update(deltaTime, players, platforms, bullets);
	for (auto& obj : coins) obj.update(deltaTime);
	flag.update(deltaTime);
}

// handle sfml events and player movement for main game
void Game::handleInput() {

	// sfml events
	sf::Event sfEvent;
	while (window.pollEvent(sfEvent)) {

		// close game -> pause menu
		if (sfEvent.type == sf::Event::KeyPressed)
			if (sfEvent.key.code == sf::Keyboard::Escape)
				gameEnded = true;


		// shooting button
		if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left)
			players[0].shoot(bullets);
	}

	// user input for first player
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) players[0].moveLeft();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) players[0].moveRight();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) players[0].moveDown();
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) players[0].jump();
}



