
#include "pch.h"
#include "platform.h"

Platform::Platform(float x, float y, sf::Texture* texture) {
	setPosition(x, y);
	setTexture(*texture);
}