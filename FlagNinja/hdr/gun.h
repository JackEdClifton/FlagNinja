#pragma once

#include "platform.h"

class Gun : public sf::Sprite {
	sf::Vector2f unitVector = { 0.0f, 0.0f };
	float angle = 0.0f;
	bool isFlipped = false;

public:
	const sf::Vector2f& getUnitVector() const;
	Gun(const char* texturePath);
	void aimWeapon(const sf::Vector2f& mousePos);
};


class Bullet : public sf::Sprite {
	sf::Vector2f unitVector;
	float speed = 300.0f;
	float startTime = 0.0f;
	const float timeout = 2.0f;

public:
	Bullet(sf::Vector2f position, sf::Vector2f unitVector, float angle);
	void update(float deltaTime);
	bool bulletTimeout(float deltaTime);
	bool isColliding(const sf::Sprite& platform);
};