#pragma once

#include "gun.h"
#include "bullet.h"

// entity base class
class Entity : public sf::Sprite {

	// animation states
	enum class Animation : int {
		stationary,
		right_1,
		right_2,
		left_1,
		left_2
	};

protected:
	// entity attributes
	sf::Texture* textures[5];
	sf::Vector2f size;
	sf::Vector2f scaleFactor = { 1.0f, 1.0f };
	sf::Vector2f vel = sf::Vector2f(0.0f, 0.0f);
	std::vector<std::pair<const sf::Sprite*, float>> collisions;

	// entity physics
	const float gravity = 640.0f;
	float maxVel = 400.0f;
	float jumpVel = -300.0f;

	// entity controls
	float maxHealth = 100.0f;
	float health = maxHealth;
	bool onFloor = false;
	int maxJumps = 2;
	int jumps = 0;
	float jumpCooldown = 0.0f;
	float shootDelay = 0.0f;
	float maxShootDelay = 0.15f;

	// entity animation
	Animation animation = Animation::stationary;
	float animationCooldown = 0.0f;
	float animationMaxCooldown = 0.15f;

	// entity actions
	virtual void initTextures() = 0;
	void resetJumpTimer(float deltaTime);
	void updateAnimation(float deltaTime);
	void updateGun(float deltaTime);

	// audio
	sf::SoundBuffer gunBuffer;
	sf::Sound gunSound;

public:
	Entity(float xPos, float yPos);

	// weapon object for entity
	Gun gun = Gun("./assets/guns/pistol.psd");

	// entity getters
	const sf::Vector2f& getVel() const;
	const sf::Vector2f& getSize() const;

	// entity actions
	void update(float deltaTime);
	void checkCollision(const float deltaTime, const sf::Sprite& target, const bool resolve = false);
	void resolveCollisions(const float deltaTime);
	void drawHealthBar(sf::RenderWindow* window);
	void shoot(std::vector<Bullet*>& bullets);
	bool hit(float damage);

	// entity controls
	void jump();
	void moveLeft();
	void moveRight();
	void moveDown();
};

// static entities (floor)
class StaticEntity : public sf::Sprite {
public:
	StaticEntity(float x, float y, sf::Texture* texture = nullptr);
};

// settings for player class
namespace settings { extern bool hardMode; }

// class for players
class Player : public Entity {
	const float maxVel = 4.0f;
	bool isPaused = false;
protected:
	sf::Vector2f scaleFactor = { 0.69f, 0.69f };
public:
	Player(float xPos, float yPos);
	float damageMultiplier = settings::hardMode ? 1.0f : 1.4f;
	void initTextures() override;
	void update(float deltaTime, const sf::Vector2f& mousePosition);
	bool getPaused() const;
	void setPaused(bool value);
	void flipPausedState();
};

// class for enemies
class Enemy : public Entity {
public:
	Enemy(float xPos, float yPos);
	void update(float deltaTime, const std::vector<Player>& players, const std::vector<StaticEntity>& platforms, std::vector<Bullet*>& bullets);
};

class BadPerson : public Enemy {
public:
	BadPerson(float xPos, float yPos);
	void initTextures() override;
};

class BadPenguin : public Enemy {
public:
	BadPenguin(float xPos, float yPos);
	void initTextures() override;
};