#pragma once


class Gun : public sf::Sprite {
	sf::Vector2f unitVector = { 0.0f, 0.0f };
	float angle = 0.0f;
	bool isFlipped = false;

public:
	Gun(const char* texturePath);
	const sf::Vector2f& getUnitVector() const;
	void aimTowards(const sf::Vector2f& target);
};

