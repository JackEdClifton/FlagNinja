
#include "pch.h"
#include "flag.h"


Flag::Flag() : StaticEntity(0.0f, 0.0f) {
	initTextures();
}

Flag::Flag(float x, float y) : StaticEntity(x, y) {
	initTextures();
}

void Flag::initTextures() {
	textures[0] = Textures::Flag_0;
	textures[1] = Textures::Flag_1;

	setTexture(*textures[0]);
}

void Flag::updateAnimation(float deltaTime) {

	if (animationCooldown > 0.0f) {
		animationCooldown -= deltaTime;
		return;
	}
	animationCooldown += animationMaxCooldown;

	// pick next animation
	animation += 1;
	if (animation >= 2)
		animation = 0;
	setTexture(*textures[animation]);
}

void Flag::update(float deltaTime) {
	updateAnimation(deltaTime);
}
