
#include "pch.h"
#include "bullet.h"


// construct bullet object
Bullet::Bullet(sf::Vector2f position, sf::Vector2f unitVector) {

	// set attributes
	setPosition(position);
	this->unitVector = unitVector;
	setTexture(*Textures::Bullet);

	// move bullet so it doesnt spawn inside the entity
	move(unitVector * 48.0f);

	// flip texture if bullet is traveling left
	if (unitVector.x < 0.0f)
		scale(-1.0f, 1.0f);

	// set the angle in degrees for the bullet
	setRotation(atan(unitVector.y / unitVector.x) * 180.0f / 3.1415926f);
}

// move the bullet each frame
void Bullet::update(float deltaTime) {
	move(unitVector * speed * deltaTime);
}

// stop the bullet traveling forever
bool Bullet::bulletTimeout(float deltaTime) {
	startTime += deltaTime;
	return startTime > timeout;
}
