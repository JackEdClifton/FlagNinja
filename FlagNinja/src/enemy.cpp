
#include "pch.h"
#include "enemy.h"

Enemy::Enemy(float xPos, float yPos) : Entity(xPos, yPos) {
	initTextures();

	gun.scale(0.7f, 0.7f);
	maxVel = 100.0f;
	maxJumps = 1;
	jumpVel = -100.0f;
}

void Enemy::initTextures() {

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


void Enemy::update(float deltaTime, const std::vector<Player>& players, const std::vector<StaticEntity>& platforms, std::vector<Bullet*>& bullets) {
	Entity::update(deltaTime);

	// point gun in direction of movement
	float x = (getVel().x < 0.0f) ? -1.0f : 1.0f;
	sf::Vector2f target = getPosition() + sf::Vector2f(500.0f * x, 0.0f);
	
	bool shootGun = false;
	float closestPlayer = 400.0f;

	// find closest player
	for (auto& player : players) {
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
	if (shootGun && closestPlayer < 200.0f)
		shoot(bullets);

	//moveRight();
	jump();
}


