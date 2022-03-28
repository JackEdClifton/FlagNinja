#pragma once

#include "timer.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "coin.h"
#include "flag.h"
#include "staticEntity.h"

// container for window settings
namespace window {
	extern const char* title;
	extern unsigned int width;
	extern unsigned int height;
	extern unsigned int style;
}

// container for game settings
namespace settings {
	extern bool isVsyncEnabled;
	extern bool hardMode;
	extern bool playMusic;

	extern bool* options[];
	const char* optionNames[];
}

// user data for a single level/map
struct Level {
	bool isUnlocked = false;
	int highScore = 0;
	float bestTime = 0.0f;

	Level(bool a, int b, float c) : isUnlocked(a), highScore(b), bestTime(c) {}
};

// user data for a collection of levels/maps
struct UserData {
	long long totalPoints = 0;
	std::vector<Level> levels;

	UserData() {
		levels.emplace_back(true, 0, 0.0f);
	}
};


// main game class
class Game {

	// constructor & destructor
public:
	Game();
	~Game();

private:
	
	// window object
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(window::width, window::height), window::title, window::style);
	

	// timer (delta time between frames)
	Timer timer;
	const float& deltaTime = timer.getDeltaTime();

	// game modes and data
	UserData userdata;
	
	enum class Mode { mainMenu, mainGame, levelSelection, settings, gameWon };
	Mode gameMode = Mode::mainMenu;
	
	enum class NetworkState { hostMenu, clientMenu, hostGame, clientGame };
	NetworkState networkMode = NetworkState::hostMenu;

	// thread for networking
	std::thread networkThread;
	bool threadActive = false;
	
	// camera
	sf::Vector2f overallCameraDisplacement = { 0.0f, 0.0f };
	float maxFallHeight = 0.0f;

	// map
	std::vector<std::string> maps;
	int currentMapIndex = 0;

	// mouse info
	bool mouseButtonDown = false;
	sf::Vector2f mousePosition;

	// user interface
	int score = 0;
	unsigned int totalCoins, collectedCoins, totalEnemies, killedEnemies;
	float totalTimer = 0.0f;

	// font
	sf::Font font;

	// audio
	sf::SoundBuffer backgroundMusicBuffer;
	sf::Sound backgroundMusic;

	// drawable objects
	std::vector<Player> players;
	std::vector<Enemy*> enemies;
	std::vector<Bullet*> bullets;
	std::vector<StaticEntity> platforms;
	std::vector<Coin> coins;
	Flag flag;

	// new objects to send to networked devices
	std::vector<Bullet*> newBullets;


	// game loops
	void mainMenu();
	void playGame(const std::string& map);
	void levelSelection();
	void settings();
	void networkLoop();

	// seperated gameloop functions
	void updateGameAttributes(); // update timer and mouse values
	void handleCollisions();  // handle collisions for all collidable objects
	void updateEntitys(); // apply velocity
	void handleInput();  // and window update position and attributes of objects

	// private methods
	void readMap(const std::string& filename);  // read and load map file
	void adjustCamera();  // move all drawable objects to keep player within window
	void resetCamera();
	void moveObjects(const sf::Vector2f& displacement);
	void drawPauseMenu();  // draw dimming and options for in-game pause menu
	void drawObjects();  // draw objects
	void drawUI();  // draw user interface
	void destroyBullet(unsigned int index);
	void destroyEnemy(unsigned int index);
	void loadUserData();
	void saveUserData();

	// networking
	static void listen(bool& threadActive);
	static void server(bool& threadActive);
	static void client(bool& threadActive);


};
