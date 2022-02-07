
#include "pch.h"

#include "platform.h"
#include "entity.h"


// constructor
Entity::Entity(float xPos, float yPos, std::string folderName) {
	pos = { xPos, yPos };
	initTextures(folderName);
}

// load textures
void Entity::initTextures(std::string folderName) {
	
	// handle texture
	textures[0].loadFromFile("./assets/" + folderName + "/default.psd");
	textures[1].loadFromFile("./assets/" + folderName + "/player_1.psd");
	textures[2].loadFromFile("./assets/" + folderName + "/player_2.psd");
	textures[3].loadFromFile("./assets/" + folderName + "/player_1.psd");
	textures[4].loadFromFile("./assets/" + folderName + "/player_2.psd");

	setTexture(textures[(int)log2<int>((int)animation)]);
	scale(scaleFactor);
	size = sf::Vector2f(getTexture()->getSize()) * scaleFactor;
}

// update variables
void Entity::updateMovement(float deltaTime) {
	// move player
	pos += vel * deltaTime;
	setPosition({ pos });

	// handle jump physics
	if (onFloor)
		jumps = maxJumps;
	onFloor = false;
	jumpCooldown = std::max(0.0f, jumpCooldown - deltaTime);
}

void Entity::updateAnimation(float deltaTime) {

	Animation lastAnimation = animation;

	// cooldown
	if (animationCooldown > 0.0f) {
		animationCooldown -= deltaTime;
		return;
	}
	animationCooldown = animationMaxCooldown;

	// moving right
	if (vel.x > 0.1f) {
		animation = animation == Animation::right_2 ? Animation::right_1 : Animation::right_2;
	}

	// moving left
	else if (vel.x < -0.1f) {
		animation = animation == Animation::left_2 ? Animation::left_1 : Animation::left_2;
	}

	// not moving
	else {
		animation = Animation::stationary;
	}

	if (animation == lastAnimation) {
		return;
	}

	setTexture(textures[(int)log2<int>((int)animation)]);
}

// collisions
void Entity::checkCollision(const float deltaTime, const Platform& target, const bool resolve) {

	// check if entity is moving
	if (this->vel.x == 0 && this->vel.y == 0) return;

	sf::Vector2f targetPos = target.getPosition();
	sf::Vector2f targetSize = sf::Vector2f(target.getTexture()->getSize());

	// expand target by player size
	targetPos -= this->size / 2.0f;
	targetSize += this->size;

	// allow player to sink in the platform a bit
	targetPos.y += 5;
	targetSize.y -= 5;

	// simulate a ray between the current position and last position
	const sf::Vector2f& ray_origin = this->pos + this->size / 2.0f;
	const sf::Vector2f& ray_dir = this->vel * deltaTime;

	// check where in the ray a collision occurred
	sf::Vector2f t_near = (targetPos - ray_origin) / ray_dir;
	sf::Vector2f t_far = (targetPos + targetSize - ray_origin) / ray_dir;

	// if any values are not a number then exit early
	if (std::isnan(t_far.y) || std::isnan(t_far.x) || std::isnan(t_near.y) || std::isnan(t_near.x)) return;

	// make sure the near time is smaller than the far time
	if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
	if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

	// if a collision has not occurred then exit early
	if (t_near.x > t_far.y || t_near.y > t_far.x) return;

	// get collision point
	float contact_time = std::max(t_near.x, t_near.y);
	float t_hit_far = std::min(t_far.x, t_far.y);

	// if the collision is behind or infront of the ray then exit early
	if (contact_time < 0.0f || contact_time >= 1.0f) return;
	if (t_hit_far < 0) return;

	// calculate the contact normal to resolve the collision
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
	else {
		collisions.push_back({ &target, contact_time });
	}
}

// resolve collisions with platform objects
void Entity::resolveCollisions(const float deltaTime) {
	std::sort(collisions.begin(), collisions.end(), [](const std::pair<const Platform*, float>& a, const std::pair<const Platform*, float>& b) { return a.second < b.second; });
	for (unsigned int i = 0; i < collisions.size(); i++)
		checkCollision(deltaTime, *collisions[i].first, true);
}

// move position of object
void Entity::cameraMoveBy(sf::Vector2f camera) {
	pos += camera;
}