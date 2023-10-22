
#include "pch.h"
#include "collectables.h"


Collectables::Collectables(float x, float y) : StaticEntity(x, y) {
	textures[0] = Textures::Health_0;
	textures[1] = Textures::Health_1;
	textures[2] = Textures::Health_0;
	textures[3] = Textures::Health_1;

	setTexture(*textures[0]);
}

void Collectables::updateAnimation(float deltaTime) {

	if (animationCooldown > 0.0f) {
		animationCooldown -= deltaTime;
		return;
	}
	animationCooldown += animationMaxCooldown;

	// pick next animation
	animation += 1;
	if (animation >= 4)
		animation = 0;
	setTexture(*textures[animation]);
}

void Collectables::update(float deltaTime) {
	updateAnimation(deltaTime);
}




