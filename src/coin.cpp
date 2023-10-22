
#include "pch.h"
#include "collectables.h"


Coin::Coin(float x, float y) : StaticEntity(x, y) {
	textures[0] = Textures::Coin_0;
	textures[1] = Textures::Coin_1;
	textures[2] = Textures::Coin_2;
	textures[3] = Textures::Coin_1;

	setTexture(*textures[0]);
}

void Coin::updateAnimation(float deltaTime) {

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

void Coin::update(float deltaTime) {
	updateAnimation(deltaTime);
}
