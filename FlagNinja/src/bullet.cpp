
#include "pch.h"
#include "bullet.h"



Bullet::Bullet(sf::Vector2f position, sf::Vector2f unitVector) {
	setPosition(position);
	move(unitVector * 64.0f);
	this->unitVector = unitVector;
	setTexture(*Textures::Bullet);
	if (unitVector.x < 0.0f)
		scale(-1.0f, 1.0f);
	setRotation(atan(unitVector.y / unitVector.x) * 180.0f / 3.1415926f);
}

void Bullet::update(float deltaTime) {
	move(unitVector * speed * deltaTime);
}

bool Bullet::bulletTimeout(float deltaTime) {
	startTime += deltaTime;
	return startTime > timeout;
}
