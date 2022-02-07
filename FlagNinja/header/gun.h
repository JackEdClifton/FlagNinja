#pragma once
#include <iostream>

#include "textuers.h"

class Gun : public sf::Sprite {
	sf::Vector2f unitVector = { 0.0f, 0.0f };
	float angle = 0.0f;
	bool isFlipped = false;

public:
	// getters
	const sf::Vector2f& getUnitVector() const { return unitVector; }

public:
	Gun(const char* texturePath) {
		setTexture(*Textures::Gun);
		scale(0.9f, 0.9f);
	}

	void aimWeapon(const sf::Vector2f& mousePos) {
		
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

};


class Bullet : public sf::Sprite {
	sf::Vector2f unitVector;
	float speed = 500.0f;
	float startTime = 0.0f;

public:

	Bullet(sf::Vector2f position, sf::Vector2f unitVector, float angle) {
		setPosition(position);
		this->unitVector = unitVector;
		setTexture(*Textures::Bullet);
		if (unitVector.x < 0.0f)
			scale(-1.0f, 1.0f);
		setRotation(angle);
	}

	void update(float deltaTime) {
		move(unitVector * speed * deltaTime);
	}

	bool bulletTimeout(float deltaTime) {
		startTime += deltaTime;
		return startTime > 5.0f;
	}
};