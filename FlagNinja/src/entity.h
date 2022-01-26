// source: https://www.youtube.com/watch?v=8JJ-4JgR7Dg

#pragma once

#include <Windows.h>
#include "platform.h"

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


class Entity : public sf::Sprite {

protected:
	sf::Texture texture = sf::Texture();
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Vector2f vel = sf::Vector2f(0.0f, 0.0f);
	std::vector<std::pair<const Platform*, float>> collisions;

	const float gravity = 640.0f;
	const float drag = 1.0f - 0.2f;
	const float maxVel = 400.0f;
	const float jumpVel = -220.0f;

	bool controlsActive = true;
	bool onFloor = false;
	short maxJumps = 2;
	short jumps = 0;
	float jumpCooldown = 0.0f;

	short animation = 1;
	float animationCooldown = 0.0f;
	float animationMaxCooldown = 0.1f;

public:

	// constructor
	Entity(const char* texturePath, float xPos = 0.0f, float yPos = 0.0f) {
		pos = { xPos, yPos };

		// handle texture
		texture.loadFromFile(texturePath);
		setTexture(texture);
		size = sf::Vector2f(getTexture()->getSize());
	}

	// update variables
	void update(float deltaTime) {
		// move player
		pos += vel * deltaTime;
		setPosition({ pos });

		// handle jump physics
		if (onFloor)
			jumps = maxJumps;
		onFloor = false;
		jumpCooldown = std::max(0.0f, jumpCooldown - deltaTime);

		// change animation
		if (abs(vel.x) > 0.1f) {
			if (animationCooldown > 0.0f) {
				animationCooldown -= deltaTime;
				return;
			}
			animationCooldown = animationMaxCooldown;
			animation += 1;
			if (animation > 2) animation = 1;
			std::string path = "./assets/player_right_";
			texture.loadFromFile(path + std::to_string(animation) + ".psd");
			setTexture(texture);
			size = sf::Vector2f(getTexture()->getSize());
		}
		else {
			texture.loadFromFile("./assets/player.psd");
			setTexture(texture);
			size = sf::Vector2f(getTexture()->getSize());
		}
	}

	void checkCollision(const float deltaTime, const Platform& target, const bool resolve = false) {

		// check rect is moving
		if (this->vel.x == 0 && this->vel.y == 0) return;

		// expand player rect
		sf::Vector2f targetPos = target.getPosition();
		sf::Vector2f targetSize = sf::Vector2f(target.getTexture()->getSize());

		targetPos -= this->size / 2.0f;
		targetSize += this->size;

		const sf::Vector2f& ray_origin = this->pos + this->size / 2.0f;
		const sf::Vector2f& ray_dir = this->vel * deltaTime;

		sf::Vector2f t_near = (targetPos - ray_origin) / ray_dir;
		sf::Vector2f t_far = (targetPos + targetSize - ray_origin) / ray_dir;

		if (std::isnan(t_far.y) || std::isnan(t_far.x) || std::isnan(t_near.y) || std::isnan(t_near.x)) return;

		if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
		if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

		if (t_near.x > t_far.y || t_near.y > t_far.x) return;

		float contact_time = std::max(t_near.x, t_near.y);
		float t_hit_far = std::min(t_far.x, t_far.y);

		if (contact_time < 0.0f || contact_time >= 1.0f) return;
		if (t_hit_far < 0) return;

		sf::Vector2f contact_normal;
		if (t_near.x > t_near.y)
			if (1.0f / ray_dir.x < 0.0f)
				contact_normal = { 1.0f, 0.0f };
			else
				contact_normal = { -1.0f, 0.0f };
		else if (t_near.x < t_near.y)
			if (1.0f / ray_dir.y < 0.0f)
				contact_normal = { 0.0f, 1.0f };
			else
				contact_normal = { 0.0f, -1.0f };
		else
			contact_normal = { 0.0f, 0.0f };

		// resolve collision
		if (resolve) {
			this->vel += contact_normal * sf::Vector2f(std::abs(this->vel.x), std::abs(this->vel.y)) * (1 - contact_time);
			
			if (contact_normal.y == -1.0f)
				onFloor = true;
		}

		// save collision for later
		else
			collisions.push_back({ &target, contact_time });
	}

	void resolveCollisions(const float deltaTime) {
		std::sort(collisions.begin(), collisions.end(), [](const std::pair<const Platform*, float>& a, const std::pair<const Platform*, float>& b) { return a.second < b.second; });
		for (unsigned int i = 0; i < collisions.size(); i++)
			checkCollision(deltaTime, *collisions[i].first, true);
	}

	void cameraMoveBy(sf::Vector2f camera) {
		pos += camera;
	}
};