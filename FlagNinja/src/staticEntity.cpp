
#include "pch.h"
#include "staticEntity.h"


StaticEntity::StaticEntity(float x, float y, sf::Texture* texture) {
	setPosition(x, y);
	if (texture)
		setTexture(*texture);
}