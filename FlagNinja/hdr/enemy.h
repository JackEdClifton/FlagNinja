#pragma once

#include "entity.h"
#include "player.h"
#include "staticEntity.h"

class Enemy : public Entity {
public:
	Enemy(float xPos, float yPos);
	void update(float deltaTime, const std::vector<Player>& players, const std::vector<StaticEntity>& platforms, std::vector<Bullet*>& bullets);
	
};


class BadPerson : public Enemy {
public:
	BadPerson(float xPos, float yPos);
	void initTextures() override;
};


class BadPenguin : public Enemy {
public:
	BadPenguin(float xPos, float yPos);
	void initTextures() override;
};