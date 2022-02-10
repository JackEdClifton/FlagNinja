
#include "pch.h"
#include "enemy.h"

Enemy::Enemy(float xPos, float yPos) : Entity(xPos, yPos) {
	initTextures();
}

void Enemy::initTextures() {

	// handle texture
	textures[0] = Textures::Enemy_1;
	textures[1] = Textures::Enemy_1;
	textures[2] = Textures::Enemy_1;
	textures[3] = Textures::Enemy_1;
	textures[4] = Textures::Enemy_1;

	setTexture(*textures[0]);
	scale(scaleFactor);
	size = sf::Vector2f(getTexture()->getSize()) * scaleFactor;
}