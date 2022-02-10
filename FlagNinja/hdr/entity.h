#pragma once

#include "gun.h"

class Entity : public sf::Sprite {

	enum class Animation : int {
		stationary = 1 << 0,
		right_1 = 1 << 1,
		right_2 = 1 << 2,
		left_1 = 1 << 3,
		left_2 = 1 << 4
	};

protected:
	sf::Texture* textures[5];
	sf::Vector2f size;
	sf::Vector2f scaleFactor = { 1.0f, 1.0f };
	sf::Vector2f vel = sf::Vector2f(0.0f, 0.0f);
	std::vector<std::pair<const sf::Sprite*, float>> collisions;

	// physics
	const float gravity = 640.0f;
	const float drag = 1.0f - 0.2f;
	const float maxVel = 400.0f;
	const float jumpVel = -220.0f;

	// controls
	bool onFloor = false;
	short maxJumps = 2;
	short jumps = 0;
	float jumpCooldown = 0.0f;

	// animation
	Animation animation = Animation::stationary;
	float animationCooldown = 0.0f;
	float animationMaxCooldown = 0.15f;

	virtual void initTextures() = 0;
	void applyExternalForces(float deltaTime);
	void resetJumpTimer(float deltaTime);
	void updateAnimation(float deltaTime);

public:

	// constructor
	Entity(float xPos, float yPos);
	Gun gun = Gun("./assets/guns/pistol.psd");

	// movement
	void update(float deltaTime);
	void checkCollision(const float deltaTime, const sf::Sprite& target, const bool resolve = false);
	void resolveCollisions(const float deltaTime);

	// move position of object
	void jump();
	void moveLeft();
	void moveRight();
	void moveDown();
};