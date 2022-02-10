#pragma once

#include "staticEntity.h"

class Coin : public StaticEntity {

	sf::Texture* textures[4];

	// animation
	int animation = 0;
	float animationCooldown = 0.0f;
	float animationMaxCooldown = 0.5f;

	void updateAnimation(float deltaTime);

public:
	Coin(float x, float y);
	void update(float deltaTime);


};