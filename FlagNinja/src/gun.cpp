
#include "pch.h"
#include "gun.h"


// getters
const sf::Vector2f& Gun::getUnitVector() const { return unitVector; }

Gun::Gun(const char* texturePath) {
	setTexture(*Textures::Gun);
	scale(0.9f, 0.9f);
}

void Gun::aimTowards(const sf::Vector2f& target) {

	// get direction vector from player to cursor
	sf::Vector2f vector = target - getPosition();

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


