#pragma once

#include "staticEntity.h"
#include "bullet.h"

std::ostream& operator<<(std::ostream&, sf::Vector2f);


namespace sf {

	extern Vector2f operator/(sf::Vector2f, sf::Vector2f);
	extern Vector2f operator*(sf::Vector2f, sf::Vector2f);
	extern Vector2f operator*=(sf::Vector2f, sf::Vector2f);

	bool isSpriteInWindow(const sf::Sprite& sprite, const sf::RenderWindow& window);
	bool isColliding(const sf::Sprite& a, const sf::Sprite& b);

	bool isViewObstructed(const sf::Sprite& a, const sf::Sprite& b, const std::vector<StaticEntity>& platforms);
}