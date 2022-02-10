#pragma once

std::ostream& operator<<(std::ostream&, sf::Vector2f);

sf::Vector2f operator/(sf::Vector2f, sf::Vector2f);
sf::Vector2f operator*(sf::Vector2f, sf::Vector2f);
sf::Vector2f operator*=(sf::Vector2f, sf::Vector2f);


namespace sf {

	bool isSpriteInWindow(const sf::Sprite& sprite, const sf::RenderWindow& window);
	bool isColliding(const sf::Sprite& a, const sf::Sprite& b);
}