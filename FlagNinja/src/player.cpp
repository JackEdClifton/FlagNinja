
#include "pch.h"
#include "player.h"


// constructor
Player::Player(float xPos, float yPos) : Entity(xPos, yPos) {
	initTextures();
}

void Player::initTextures() {
	textures[0] = Textures::Player_0;
	textures[1] = Textures::Player_1;
	textures[2] = Textures::Player_2;
	textures[3] = Textures::Player_3;
	textures[4] = Textures::Player_4;

	setTexture(*textures[0]);
	scale(scaleFactor);
	size = sf::Vector2f(getTexture()->getSize()) * scaleFactor;
}

void Player::update(float deltaTime, const sf::Vector2f& mousePosition) {
	if (isPaused) return;
	Entity::update(deltaTime);
	gun.aimTowards(mousePosition);
}

bool Player::getPaused() const { return isPaused; }
void Player::setPaused(bool value) { isPaused = value; }
void Player::flipPausedState() { isPaused = !isPaused; }
