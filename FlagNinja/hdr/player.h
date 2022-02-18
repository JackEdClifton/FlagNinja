#pragma once

#include "entity.h"


class Player : public Entity {
	const float maxVel = 4.0f;
	bool isPaused = false;

protected:
	sf::Vector2f scaleFactor = { 0.69f, 0.69f };
	
public:
	
	// constructor
	Player(float xPos, float yPos);

	void initTextures() override;
	void update(float deltaTime, const sf::Vector2f& mousePosition);

	bool getPaused() const;
	void setPaused(bool value);
	void flipPausedState();
};