#pragma once

#include <thread>
#include <Windows.h>
#include "platform.h"

struct Collision {
	const Platform* platform;
	float time;
	sf::Vector2f contact_normal;
	sf::Vector2f contact_point;

	Collision(const Platform* platform, const float time, const sf::Vector2f contact_normal, const sf::Vector2f contact_point) {
		this->platform = platform;
		this->time = time;
		this->contact_normal = contact_normal;
		this->contact_point = contact_point;
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

	sf::Vector2f pos = getPosition();
	sf::Vector2f size;
	sf::Vector2f vel = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f lastPos = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f movementVector;

	std::vector<Collision> collidingPlatforms;

	float mass = 5.0f;
	float drag = 0.995f;
	float maxVel = 30.0f;
	float jumpVel = -40.0f;

	bool controlsActive = true;
	bool isColliding = true;

	void resolveCollisions() {
		std::sort(
			collidingPlatforms.begin(),
			collidingPlatforms.end(),
			[](const Collision& a, const Collision& b) { return a.time < b.time; }
		);

		for (const Collision& collision : collidingPlatforms) {
			sf::Vector2f distance = (collision.contact_point - pos) * (collision.contact_normal);
			std::cout << "Distance:" << distance.x << ", " << distance.y << std::endl;

			if (std::isnan(distance.x) || std::isnan(distance.y))
				return;
			pos += distance;
		}
	}

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

		// clear colliding platforms
		resolveCollisions();
		collidingPlatforms.clear();

		// update object attributes
		setPosition(pos.x, pos.y);
		lastPos = pos;

		// ignore if in pause menu or somethn
		if (!controlsActive) { return; }

		// simulate drag
		vel.x *= timeDelta * drag;
		vel.y *= timeDelta * drag;

		// add velocity
		vel.y += mass * 9.8f * timeDelta;

		// jumping
		//if ((GetAsyncKeyState('W') || GetAsyncKeyState(' ')) && isColliding) { vel.y = jumpVel; }

		// moving x axis
		if (GetAsyncKeyState('A')) { vel.x = -maxVel; }
		if (GetAsyncKeyState('D')) { vel.x = maxVel; }
		if (GetAsyncKeyState('W')) { vel.y = -maxVel; }  // temp for debugging
		if (GetAsyncKeyState('S')) { vel.y = maxVel; }  // temp for debugging

		// stops moving at low speeds for ages
		vel.x *= (abs(vel.x) > 0.1f);
		vel.y *= (abs(vel.y) > 0.1f);

		// move player
		movementVector.x = vel.x * timeDelta;
		movementVector.y = vel.y * timeDelta;
		pos.x += movementVector.x;
		pos.y += movementVector.y;
	}


#define ray_origin lastPos
#define ray_dir movementVector
	void olcRayVsRect(const Platform& target) {

		sf::Vector2f contact_point, contact_normal;
		float t_hit_near;

		sf::Vector2f targetPos = target.getPosition();
		sf::Vector2f targetSize = sf::Vector2f(target.getTexture()->getSize());

		// expand platform by size of player
		targetPos -= size;
		targetSize += size;

		sf::Vector2f t_near = (targetPos - ray_origin) / ray_dir;
		sf::Vector2f t_far = (targetPos + targetSize - ray_origin) / ray_dir;

		if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
		if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

		if (t_near.x > t_far.y || t_near.y > t_far.x) return;

		t_hit_near = std::max(t_near.x, t_near.y);
		float t_hit_far = std::min(t_far.x, t_far.y);

		if (t_hit_far < 0) return;


		contact_point = ray_origin + t_hit_near * ray_dir;

		if (t_near.x > t_near.y)
			if (ray_dir.x < 0)	contact_normal = { 1, 0 };	// + colliding right
			else				contact_normal = { 1, 0 };	// - colliding left
		else if (t_near.x < t_near.y)
			if (ray_dir.y < 0)	contact_normal = { 0, 1 };	// + colliding base
			else				contact_normal = { 0, 1 };	// - colliding top

		// add rect to std::vector of colliding platforms
		if (t_hit_near <= 1.0f)
			collidingPlatforms.push_back(Collision(&target, t_hit_near, contact_normal, contact_point));
	}

};