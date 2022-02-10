#pragma once

#include "entity.h"

class Enemy : public Entity {

public:
	Enemy(float xPos, float yPos);
	void initTextures() override;
};