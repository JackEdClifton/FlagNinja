#pragma once


class Entity : public sf::Sprite {

	enum class Animation : int {
		stationary = 1 << 0,
		left_1 = 1 << 1,
		left_2 = 1 << 2,
		right_1 = 1 << 3,
		right_2 = 1 << 4
	};

protected:
	sf::Texture textures[5];
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Vector2f scaleFactor = { 0.69f, 0.69f };
	sf::Vector2f vel = sf::Vector2f(0.0f, 0.0f);
	std::vector<std::pair<const Platform*, float>> collisions;

	// physics
	const float gravity = 640.0f;
	const float drag = 1.0f - 0.2f;
	const float maxVel = 400.0f;
	const float jumpVel = -220.0f;

	// controls
	bool controlsActive = true;
	bool onFloor = false;
	short maxJumps = 2;
	short jumps = 0;
	float jumpCooldown = 0.0f;

	// animation
	Animation animation = Animation::stationary;
	float animationCooldown = 0.0f;
	float animationMaxCooldown = 0.1f;

public:

	// constructor
	Entity(float xPos, float yPos, std::string folderName);

	void initTextures(std::string folderName);

	// update variables
	void updateMovement(float deltaTime);

	void updateAnimation(float deltaTime);

	// collisions
	void checkCollision(const float deltaTime, const Platform& target, const bool resolve = false);
	void resolveCollisions(const float deltaTime);

	// move position of object
	void cameraMoveBy(sf::Vector2f camera);
};