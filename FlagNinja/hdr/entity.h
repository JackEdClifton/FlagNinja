#pragma once

#include "gun.h"
#include "bullet.h"

class Entity : public sf::Sprite {

	enum class Animation : int {
		stationary,
		right_1,
		right_2,
		left_1,
		left_2
	};

protected:
	sf::Texture* textures[5];
	sf::Vector2f size;
	sf::Vector2f scaleFactor = { 1.0f, 1.0f };
	sf::Vector2f vel = sf::Vector2f(0.0f, 0.0f);
	std::vector<std::pair<const sf::Sprite*, float>> collisions;

	// physics
	const float gravity = 640.0f;
	float maxVel = 400.0f;
	float jumpVel = -300.0f;

	// controls
	float maxHealth = 100.0f;
	float health = maxHealth;
	bool onFloor = false;
	int maxJumps = 2;
	int jumps = 0;
	float jumpCooldown = 0.0f;
	float shootDelay = 0.0f;
	float maxShootDelay = 0.1f;

	// animation
	Animation animation = Animation::stationary;
	float animationCooldown = 0.0f;
	float animationMaxCooldown = 0.15f;

	virtual void initTextures() = 0;
	void resetJumpTimer(float deltaTime);
	void updateAnimation(float deltaTime);
	void updateGun(float deltaTime);

public:

	Entity(float xPos, float yPos);

	Gun gun = Gun("./assets/guns/pistol.psd");

	const sf::Vector2f& getVel() const;
	const sf::Vector2f& getSize() const;

	void update(float deltaTime);
	void checkCollision(const float deltaTime, const sf::Sprite& target, const bool resolve = false);
	void resolveCollisions(const float deltaTime);
	void drawHealthBar(sf::RenderWindow* window);
	void shoot(std::vector<Bullet*>& bullets);
	bool hit(float damage);

	void jump();
	void moveLeft();
	void moveRight();
	void moveDown();
};