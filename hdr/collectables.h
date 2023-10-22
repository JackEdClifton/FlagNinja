#pragma once

#include "entity.h"

// coin class
class Coin : public StaticEntity {

	// textures
	sf::Texture* textures[4];

	// animation
	int animation = 0;
	float animationCooldown = 0.0f;
	float animationMaxCooldown = 0.2f;
	void updateAnimation(float deltaTime);

public:
	Coin(float x, float y);
	void update(float deltaTime);
};

// collectables class
class Collectables : public StaticEntity {

	// textures
	sf::Texture* textures[4];

	// animation
	int animation = 0;
	float animationCooldown = 0.0f;
	float animationMaxCooldown = 0.2f;
	void updateAnimation(float deltaTime);

public:
	Collectables(float x, float y);
	void update(float deltaTime);
};