#pragma once
#include <iostream>

#include "textuers.h"

class Platform : public sf::Sprite {
public:
	Platform(float x, float y, std::string _) {
		setPosition(x, y);
		setTexture(*Textures::Platform);
	}

};