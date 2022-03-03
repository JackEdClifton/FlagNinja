
#include "pch.h"
#include "entity.h"

#include "colour.h"

// construct entity object
Entity::Entity(float xPos, float yPos) {
	setPosition(xPos, yPos);
	health = maxHealth;
}

// getter for velocity
const sf::Vector2f& Entity::getVel() const { return vel; }
const sf::Vector2f& Entity::getSize() const { return size; }

// if the player is on the floor they should be able to jump again
void Entity::resetJumpTimer(float deltaTime) {
	if (onFloor)
		jumps = maxJumps;
	onFloor = false;
	jumpCooldown = std::max(0.0f, jumpCooldown - deltaTime);
}

// set the texture so the entity looks like its walking
void Entity::updateAnimation(float deltaTime) {

	Animation lastAnimation = animation;

	// cooldown
	if (animationCooldown > 0.0f) {
		animationCooldown -= deltaTime;
		return;
	}
	animationCooldown += animationMaxCooldown;

	// pick next animation
	if (vel.x > 0.1f) animation = animation == Animation::right_2 ? Animation::right_1 : Animation::right_2;
	else if (vel.x < -0.1f) animation = animation == Animation::left_2 ? Animation::left_1 : Animation::left_2;
	else animation = Animation::stationary;

	if (animation == lastAnimation) return;

	//setTexture(*textures[(int)log2<int>((int)animation)]);
	setTexture(*textures[(int)animation]);
}

void Entity::updateGun(float deltaTime) {
	gun.setPosition(getPosition() + size / sf::Vector2f(2.0f, 5.0f));
	shootDelay -= deltaTime;
	if (shootDelay < 0.0f)
		shootDelay = 0.0f;
}

// wrapper for other update functions
void Entity::update(float deltaTime) {
	move(vel * deltaTime);
	updateAnimation(deltaTime);
	updateGun(deltaTime);
	vel.x = 0.0f;
	vel.y += (vel.y < 0.0f ? 1.0f : 2.5f) * gravity * deltaTime;
	resetJumpTimer(deltaTime);
}

// swept AABB collisions
void Entity::checkCollision(const float deltaTime, const sf::Sprite& target, const bool resolve) {

	// check if entity is moving
	if (vel.x == 0 && vel.y == 0) return;

	// cache values
	sf::Vector2f targetPos = target.getPosition();
	sf::Vector2f targetSize = sf::Vector2f(target.getTexture()->getSize());

	// expand target by player size
	targetPos -= size / 2.0f;
	targetSize += size;

	// allow player to sink in the platform a bit
	targetPos.y += 5;
	targetSize.y -= 5;

	// simulate a ray between the current position and next position
	const sf::Vector2f& ray_origin = getPosition() + (size / 2.0f);
	const sf::Vector2f& ray_dir = vel * deltaTime;

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
		vel += contact_normal * sf::Vector2f(std::abs(vel.x), std::abs(vel.y)) * (1.0f - contact_time);
		if (contact_normal.y == -1.0f) { onFloor = true; }
	}
	else {
		collisions.emplace_back(&target, contact_time);
	}
}

// resolve collisions with platform objects
void Entity::resolveCollisions(const float deltaTime) {
	std::sort(collisions.begin(), collisions.end(), [](
		const std::pair<const sf::Sprite*, float>& a,
		const std::pair<const sf::Sprite*, float>& b
		) { return a.second < b.second; }
	);
	for (unsigned int i = 0; i < collisions.size(); i++)
		checkCollision(deltaTime, *collisions[i].first, true);
	collisions.clear();
}

// draw health bar
void Entity::drawHealthBar(sf::RenderWindow* window) {
	sf::RectangleShape healthBar({ 50.0f, 5.0f });
	
	healthBar.setPosition(getPosition() + sf::Vector2f(0.0f, -10.0f));
	healthBar.setFillColor(Colour::HealthBar::Damaged);
	window->draw(healthBar);
	
	healthBar.setSize(sf::Vector2f((health / maxHealth) * 50.0f, 5.0f));
	healthBar.setFillColor(Colour::HealthBar::Normal);
	window->draw(healthBar);
}

// spawn a bullet obj
void Entity::shoot(std::vector<Bullet*>& bullets) {
	if (shootDelay == 0.0f) {
		bullets.push_back(new Bullet(gun.getPosition(), gun.getUnitVector()));
		shootDelay = maxShootDelay;
	}
}

bool Entity::hit(float damage) {
	health -= damage;
	return health <= 0.0f;
}

// control the entitys movement
void Entity::jump() {
	if (jumps && !jumpCooldown) {
		vel.y = jumpVel;
		jumps -= 1;
		jumpCooldown = 0.2f;
	}
}
void Entity::moveDown() {
	if (vel.y < maxVel)
		vel.y = maxVel;
	jumps = 0;
}
void Entity::moveLeft() { vel.x = -maxVel; }
void Entity::moveRight() { vel.x = maxVel; }


