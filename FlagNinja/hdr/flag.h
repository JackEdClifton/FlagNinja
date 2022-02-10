#pragma once

#include "staticEntity.h"

class Flag : public StaticEntity {
	sf::Texture* textures[2];

	// animation
	int animation = 0;
	float animationCooldown = 0.0f;
	float animationMaxCooldown = 0.5f;

	void initTextures();
	void updateAnimation(float deltaTime);

public:
	Flag();
	Flag(float x, float y);
	void update(float deltaTime);


};