
#include "pch.h"
#include "game.h"

#include "colour.h"
#include "button.h"

// container for window attributes
namespace window {
	const char* title = "Flag Ninja";

#if _DEBUG
	unsigned int width = 1400;
	unsigned int height = 600;
	unsigned int style = sf::Style::Close;
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
	const char* optionNames[] = { "V Sync", "Hard Mode", "Background Music" };
}

// game constructor
Game::Game() {

	// load font
	font.loadFromFile("./assets/fonts/comic.ttf");

	// load background audio
	backgroundMusicBuffer.loadFromFile("./assets/audio/project 1.wav");
	backgroundMusic.setBuffer(backgroundMusicBuffer);
	backgroundMusic.setVolume(20.0f);
	backgroundMusic.setLoop(true);

	// apply settings
	window.setVerticalSyncEnabled(settings::isVsyncEnabled);
	if (settings::playMusic) backgroundMusic.play();

	// load maps
	for (auto& map : std::filesystem::directory_iterator("./assets/maps"))
		maps.push_back(map.path().filename().string());

	// pad user data to fit amount of maps
	loadUserData();
	while (userdata.levels.size() < maps.size())
		userdata.levels.emplace_back(false, 0, 0.0f);


	// launch game
	mainMenu();
}

// game deconstructor
Game::~Game() {

	// save data for user progress in external file
	saveUserData();

	// clean up memory
	for (auto& obj : enemies) delete obj;
	for (auto& obj : bullets) delete obj;
	Textures::destroy();
	
	// end network threads
	threadActive = false;
	networkThread.joinable() ? networkThread.join() : void();

	// make sure window is closed
	window.close();
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
			
			// platforms
			else if (chr == 'g') platforms.emplace_back(x, y, Textures::Grass);
			else if (chr == 'd') platforms.emplace_back(x, y, Textures::Dirt);
			else if (chr == 's') platforms.emplace_back(x, y, Textures::Snow);
			else if (chr == 'b') platforms.emplace_back(x, y, Textures::SnowDirt);

			// env
			else if (chr == 'f') flag.setPosition(x, y);
			else if (chr == 'c') {
				coins.emplace_back(x, y);
				totalCoins += 1;
			}

			// player
			else if (chr == 'p') {
				players.emplace_back(x, y);
				containsPlayer = true;
			}

			// enemies
			else if (chr == '1') {
				enemies.push_back(new BadPerson(x, y));
				totalEnemies += 1;
			}
			else if (chr == '2') {
				enemies.push_back(new BadPenguin(x, y));
				totalEnemies += 1;
			}
		}
		y += step;
	}
	file.close();

	// maximun height user can fall before dying
	maxFallHeight = step + 3000.0f;

	// log an error if a player has not been created
	if (!containsPlayer) {
		std::cerr << "WARNING! - No player detected when reading file (" << filename << ")\n";
		players.emplace_back(0.0f, 0.0f); // create temporary player
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
	if (gameMode == Mode::mainGame)
		totalTimer += deltaTime;
	mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	mouseButtonDown = false;
}

// move all drawable objects by a fixed amount
void Game::moveObjects(const sf::Vector2f& displacement) {
	for (auto& obj : players) obj.move(displacement);
	for (auto& obj : enemies) obj->move(displacement);
	for (auto& obj : bullets) obj->move(displacement);
	for (auto& obj : platforms) obj.move(displacement);
	for (auto& obj : coins) obj.move(displacement);
	flag.move(displacement);
	overallCameraDisplacement += displacement;
	maxFallHeight += displacement.y;
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


// find collisions and handle them apropriately
void Game::handleCollisions() {

	// validate game mode
	if (gameMode != Mode::mainGame)
		return;

	// players
	for (auto& player : players) {
		if (player.getPaused()) continue;
		for (auto& platform : platforms)
			player.checkCollision(deltaTime, platform);
		player.resolveCollisions(deltaTime);

		// if player has fell out of world exit game
		if (player.getPosition().y > maxFallHeight)
			gameMode = Mode::mainMenu;
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

			if (player.getPaused())
				continue;

			if (isColliding(*bullets[i], player)) {
				if (player.hit(0.3f * player.damageMultiplier)) {
					gameMode = Mode::mainMenu;
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
		continue;
	exitBulletLoop:
		i--;
	}

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
			score += 500;
			gameMode = Mode::gameWon;
		}
	}
}

// call update method for game objects
void Game::updateEntitys() {

	if (gameMode != Mode::mainGame) return;

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

		// force exit game
		if (sfEvent.type == sf::Event::Closed)
			window.close();

		// close game -> pause menu
		if (sfEvent.type == sf::Event::KeyPressed)
			if (sfEvent.key.code == sf::Keyboard::Escape)
				players[0].flipPausedState();


		// shooting button
		if (sfEvent.type == sf::Event::MouseButtonPressed && sfEvent.mouseButton.button == sf::Mouse::Left) {
			players[0].shoot(bullets);
			mouseButtonDown = true;
		}
	}

	// user input for first player
	if (!players[0].getPaused()) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) players[0].moveLeft();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) players[0].moveRight();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) players[0].moveDown();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) players[0].jump();
	}
}


