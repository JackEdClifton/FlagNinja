
#include "pch.h"
#include "entity.h"
#include "settings.h"


// enemy constructor
Enemy::Enemy(float xPos, float yPos) : Entity(xPos, yPos) {}

// move enemy and aim gun
void Enemy::update(float deltaTime, const std::vector<Player>& players, const std::vector<StaticEntity>& platforms, std::vector<Bullet*>& bullets) {
	Entity::update(deltaTime);

	// point gun in direction of movement
	float x = (getVel().x < 0.0f) ? -1.0f : 1.0f;
	sf::Vector2f target = getPosition() + sf::Vector2f(500.0f * x, 0.0f);
	
	bool shootGun = false;
	float aimDistance = 120.0f;
	float minShootDistance = 400.0f;
	if (settings::hardMode)
		minShootDistance = 600.0f;

	float closestPlayer = minShootDistance;

	// find closest player
	for (auto& player : players) {

		if (player.getPaused())
			continue;

		if (!sf::isViewObstructed(*this, player, platforms)) {

			const sf::Vector2f& distanceVector = player.getPosition() - getPosition();
			float distance = sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);

			if (distance < closestPlayer) {
				closestPlayer = distance;
				target = player.getPosition();
				shootGun = true;
			}

		}
	}
	// aim gun and shoot player
	gun.aimTowards(target);
	if (shootGun && closestPlayer < minShootDistance - aimDistance) {
		
		shoot(bullets);

		// move left
		if (target.x < getPosition().x - 64.0f) {
			moveLeft();
		}
		// move right
		else  if (target.x > getPosition().x + 96.0f) {
			moveRight();
		}
	}
}


// bad person constructor
BadPerson::BadPerson(float xPos, float yPos) : Enemy(xPos, yPos) {
	initTextures();

	maxVel = 100.0f;
	maxJumps = 1;
	jumpVel = -100.0f;
}

// load textures for bad person
void BadPerson::initTextures() {

	// handle texture
	textures[0] = Textures::Enemy_1;
	textures[1] = Textures::Enemy_1;
	textures[2] = Textures::Enemy_1;
	textures[3] = Textures::Enemy_1;
	textures[4] = Textures::Enemy_1;

	setTexture(*textures[0]);
	scale(scaleFactor);
	size = sf::Vector2f(getTexture()->getSize()) * scaleFactor;
}


// bad penguin constructor
BadPenguin::BadPenguin(float xPos, float yPos) : Enemy(xPos, yPos) {
	initTextures();

	maxVel = 100.0f;
	maxJumps = 1;
	jumpVel = -100.0f;
	health = maxHealth = 750.0f;
}

// load textures for bad penguin
void BadPenguin::initTextures() {

	// handle texture
	textures[0] = Textures::BadPeng;
	textures[1] = Textures::BadPeng;
	textures[2] = Textures::BadPeng;
	textures[3] = Textures::BadPeng;
	textures[4] = Textures::BadPeng;

	setTexture(*textures[0]);
	scale(scaleFactor);
	size = sf::Vector2f(getTexture()->getSize()) * scaleFactor;
}