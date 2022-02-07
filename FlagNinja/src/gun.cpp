
#include <iostream>
#include "pch.h"

#include "platform.h"
#include "gun.h"


// getters
const sf::Vector2f& Gun::getUnitVector() const { return unitVector; }

Gun::Gun(const char* texturePath) {
	setTexture(*Textures::Gun);
	scale(0.9f, 0.9f);
}

void Gun::aimWeapon(const sf::Vector2f& mousePos) {

	// get direction vector from player to cursor
	sf::Vector2f vector = mousePos - getPosition();

	// set unit vector
	if (vector.x || vector.y)
		unitVector = vector / sqrt(vector.x * vector.x + vector.y * vector.y);

	// set angle
	angle = atan(vector.y / vector.x) * 180.0f / 3.1415926f;
	setRotation(angle);

	// flip image on x axis
	if (vector.x < 0.0f && !isFlipped || vector.x > 0.0f && isFlipped) {
		scale(-1.0f, 1.0f);
		isFlipped = !isFlipped;
	}

}




Bullet::Bullet(sf::Vector2f position, sf::Vector2f unitVector, float angle) {
	setPosition(position);
	this->unitVector = unitVector;
	setTexture(*Textures::Bullet);
	if (unitVector.x < 0.0f)
		scale(-1.0f, 1.0f);
	setRotation(angle);
}

void Bullet::update(float deltaTime) {
	move(unitVector * speed * deltaTime);
}

bool Bullet::bulletTimeout(float deltaTime) {
	startTime += deltaTime;
	return startTime > timeout;
}

bool Bullet::isColliding(const sf::Sprite& target) {
	sf::Vector2f thisPos = (sf::Vector2f)this->getTexture()->getSize();

	sf::Vector2f targetPos = target.getPosition();
	sf::Vector2f targetSize = (sf::Vector2f)(target.getTexture()->getSize());

	return (thisPos.x > targetPos.x) && (thisPos.x < targetPos.x + targetSize.x)
		&& (thisPos.y > targetPos.y) && (thisPos.y < targetPos.y + targetSize.y);
}