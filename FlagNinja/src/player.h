#pragma once

#include <thread>
#include <Windows.h>
#include "platform.h"

// pre-compiler definitions for directions
#define left	0b0001
#define right	0b0010
#define top		0b0100
#define base	0b1000


struct Line {
	float m, c;

	char isColliding(const sf::Vector2f& pos, const sf::Vector2f& size) {
		float y1 = m * pos.x + c;
		float y2 = m * (pos.x + size.x) + c;

		char sides = 0;

		// intersects left side
		sides |= left * (y1 > pos.y && y1 < pos.y + size.y);

		// intersects right side
		sides |= right * (y2 > pos.y && y2 < pos.y + size.y);

		// intersects top side
		sides |= top * (y1 < pos.y&& y2 > pos.y);

		// intersects base side
		sides |= base * (y1 > pos.y && y2 < pos.y);

		return sides;
	}
};

class Player : public sf::Sprite {

	sf::Vector2f pos = getPosition();
	sf::Vector2f size;
	sf::Vector2f vel = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f lastPos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f movementVector;

	float mass = 5.0f;
	float drag = 0.995f;
	float maxVel = 30.0f;
	float jumpVel = -100.0f;

	bool controlsActive = true;
	bool isColliding = true;


public:

	// constructor
	Player(const char* texturePath) {

		// handle texture
		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile(texturePath);
		setTexture(*texture);
		size = sf::Vector2f(getTexture()->getSize());
	}

	// handle position
	void update(float timeDelta) {

		std::cout << "Position: " << pos.x << ", " << pos.y << std::endl;

		// update object attributes
		lastPos = pos;

		// ignore if in pause menu or somethn
		if (!controlsActive) { return; }

		// simulate drag
		vel.x *= timeDelta * drag;
		vel.y *= timeDelta * drag;

		// add velocity
		vel.y += mass * 9.8f * timeDelta;

		// jumping
		if ((GetAsyncKeyState('W') || GetAsyncKeyState(' ')) && isColliding) {
			vel.y = jumpVel;
		}

		// moving x axis
		if (GetAsyncKeyState('A')) { vel.x = -maxVel; }
		if (GetAsyncKeyState('D')) { vel.x = maxVel; }

		// stops moving at low speeds for ages
		vel.x *= (vel.x > 0.1f);
		vel.y *= (vel.y > 0.1f);

		// move player
		movementVector.x = vel.x * timeDelta;
		movementVector.y = vel.y * timeDelta;
		pos.x += movementVector.x;
		pos.y += movementVector.y;
		setPosition(pos.x, pos.y);


	}

	// handle collisions
	void checkCollisions(const Platform& platform) {

		// platform
		sf::Vector2f platformPos = platform.getPosition();
		sf::Vector2f platformSize = sf::Vector2f(platform.getTexture()->getSize());
		
		// expand platform by size of player
		platformPos -= size;
		platformSize += size;
		
		// get equation of velocity
		Line velocityLine;
		velocityLine.m = vel.y / vel.x;
		velocityLine.c = pos.y - (velocityLine.m * pos.x);

		// get contact points
		char collisionSides = velocityLine.isColliding(platformPos, platformSize);

		// make sure there is a collision
		int totalSidesCollided = 0;
		for (char mask = 1; mask; mask <= 1)
			if (collisionSides & mask)
				totalSidesCollided += 1;

		if (totalSidesCollided == 0) return;
		if (totalSidesCollided == 1) std::cerr << "ERROR:  1 sides of collision";
		if (totalSidesCollided == 3) std::cerr << "ERROR:  3 sides of collision";
		if (totalSidesCollided == 4) std::cerr << "ERROR:  3 sides of collision";

		// get closest contact point from lastPos
		if (collisionSides & (left | right))
			if (vel.x > 0.0f)
				pos.x = platformPos.x;
			else
				pos.x = platformPos.x + platformSize.x;

		if (collisionSides & (top | base))
			if (vel.y > 0.0f)
				pos.y = platformPos.y;
			else
				pos.y = platformPos.y + platformSize.y;

		// idk wtf im doing

		// add rect to std::vector of colliding platforms

		// check if not colliding - old, allows tunneling
		if (pos.x > platformPos.x + platformSize.x) { return; }
		if (pos.x + size.x < platformPos.x) { return; }
		if (pos.y > platformPos.y + platformSize.y) { return; }
		if (pos.y + size.y < platformPos.y) { return; }

	}

};