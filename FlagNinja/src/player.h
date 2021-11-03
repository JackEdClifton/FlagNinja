#pragma once

#include <Windows.h>
#include "platform.h"

struct Collision {
	const Platform* platform;
	float time;
	sf::Vector2f contactNormal;
	sf::Vector2f contactPoint;

	Collision(const Platform* platform, const float time, const sf::Vector2f contactNormal, const sf::Vector2f contactPoint) {
		this->platform = platform;
		this->time = time;
		this->contactNormal = contactNormal;
		this->contactPoint = contactPoint;
	}
};

// override devision operator
sf::Vector2f operator/(sf::Vector2f a, sf::Vector2f b) {
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

// override multiplication operator
sf::Vector2f operator*(sf::Vector2f a, sf::Vector2f b) {
	a.x *= b.x;
	a.y *= b.y;
	return a;
}


class Player : public sf::Sprite {

	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Vector2f vel = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f lastPos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f movementVector;

	std::vector<Collision> collidingPlatforms;

	float mass = 10.0f;
	float drag = 0.995f;
	float maxVel = 50.0f;
	float jumpVel = -400.0f;
	float jumpCoolDownTime = 0.0f;
	float jumpSetCoolDownTime = 2.0f;

	bool controlsActive = true;
	bool isColliding = false;

	void resolveCollisions() {

		// exit if there are no collisons
		if (!collidingPlatforms.size())
			return;

		isColliding = true;
		
		// closest collision must be processed first
		std::sort(
			collidingPlatforms.begin(),
			collidingPlatforms.end(),
			[](const Collision& a, const Collision& b) { return a.time < b.time; }
		);

		for (const Collision& collision : collidingPlatforms) {
			sf::Vector2f distance = (collision.contactPoint - pos) * (collision.contactNormal);

			if (std::isnan(distance.x) || std::isnan(distance.y))
				return;
			pos += distance;
		}

		collidingPlatforms.clear();
	}

public:

	// constructor
	Player(const char* texturePath, float xPos = 0.0f, float yPos = 0.0f) {

		pos = { xPos, yPos };

		// handle texture
		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile(texturePath);
		setTexture(*texture);
		size = sf::Vector2f(getTexture()->getSize());
	}

	// handle position
	void update(float timeDelta) {

		// clear colliding platforms
		isColliding = false;
		resolveCollisions();

		// update object attributes
		setPosition(pos.x, pos.y);
		lastPos = pos;

		// ignore if in pause menu or somethn
		if (!controlsActive) { return; }

		// simulate drag
		vel.x *= timeDelta * drag;
		vel.y *= timeDelta * drag;

		// gravity
		vel.y += mass * 9.8f * timeDelta;

		// jumping
		jumpCoolDownTime -= timeDelta * bool(jumpCoolDownTime);
		jumpCoolDownTime *= jumpCoolDownTime >= 0.0f;

		if ((GetAsyncKeyState('W') || GetAsyncKeyState(' ')) && isColliding && !jumpCoolDownTime) {
			vel.y = jumpVel;
			jumpCoolDownTime = jumpSetCoolDownTime;
		}

		// moving x axis
		if (GetAsyncKeyState('A')) { vel.x = -maxVel; }
		if (GetAsyncKeyState('D')) { vel.x = maxVel; }

		// stops moving at low speeds for ages
		vel.x *= (abs(vel.x) > 0.1f);
		vel.y *= (abs(vel.y) > 0.1f);

		// move player
		movementVector.x = vel.x * timeDelta;
		movementVector.y = vel.y * timeDelta;
		pos.x += movementVector.x;
		pos.y += movementVector.y;
	}


	// source: https://www.youtube.com/watch?v=8JJ-4JgR7Dg
	void checkCollision(const Platform& target) {

		sf::Vector2f contactPoint, contactNormal;
		float tHitNear;

		sf::Vector2f targetPos = target.getPosition();
		sf::Vector2f targetSize = sf::Vector2f(target.getTexture()->getSize());

		// expand platform by size of player
		targetPos -= size;
		targetSize += size;

		sf::Vector2f tNear = (targetPos - lastPos) / movementVector;
		sf::Vector2f tFar = (targetPos + targetSize - lastPos) / movementVector;

		if (tNear.x > tFar.x) std::swap(tNear.x, tFar.x);
		if (tNear.y > tFar.y) std::swap(tNear.y, tFar.y);

		if (tNear.x > tFar.y || tNear.y > tFar.x) return;

		tHitNear = std::max(tNear.x, tNear.y);
		float tHitFar = std::min(tFar.x, tFar.y);

		if (tHitFar <= 0) return;


		contactPoint = lastPos + tHitNear * movementVector;

		if (tNear.x > tNear.y)
			contactNormal = { 1, 0 };
			
		else if (tNear.x < tNear.y)
			contactNormal = { 0, 1 };
			
		// add rect to std::vector of colliding platforms
		if (tHitNear <= 1.0f)
			collidingPlatforms.push_back(Collision(&target, tHitNear, contactNormal, contactPoint));
	}

};