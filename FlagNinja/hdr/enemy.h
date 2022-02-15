#pragma once

#include "entity.h"
#include "player.h"
#include "staticEntity.h"

class Enemy : public Entity {
public:
	Enemy(float xPos, float yPos);
	void initTextures() override;
	void update(float deltaTime, const std::vector<Player>& players, const std::vector<StaticEntity>& platforms, std::vector<Bullet*>& bullets);
	
};