#pragma once

class Bullet : public sf::Sprite {
	sf::Vector2f unitVector;
	float speed = 700.0f;
	float startTime = 0.0f;
	const float timeout = 2.0f;

public:
	Bullet(sf::Vector2f position, sf::Vector2f unitVector);
	void update(float deltaTime);
	bool bulletTimeout(float deltaTime);
};