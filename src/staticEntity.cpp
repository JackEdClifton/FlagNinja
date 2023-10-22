
#include "pch.h"
#include "entity.h"


StaticEntity::StaticEntity(float x, float y, sf::Texture* texture) {
	setPosition(x, y);
	if (texture)
		setTexture(*texture);
}