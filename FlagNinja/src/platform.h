#pragma once

class Platform : public sf::Sprite {

public:
	Platform(float x, float y, const char* texturePath) {
		
		setPosition(x, y);

		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile(texturePath);
		setTexture(*texture);
	}

};