
#include "pch.h"

// output operator
std::ostream& operator<<(std::ostream& stream, sf::Vector2f vector) {
	stream << "(" << vector.x << ", " << vector.y << ")";
	return stream;
}


// division operator
sf::Vector2f operator/(sf::Vector2f a, sf::Vector2f b) {
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

// multiplication operator
sf::Vector2f operator*(sf::Vector2f a, sf::Vector2f b) {
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

// multiplication assignment operator
sf::Vector2f operator*=(sf::Vector2f a, sf::Vector2f b) {
	a = a * b;
	return a;
}


namespace sf {

	bool isSpriteInWindow(const sf::Sprite& sprite, const sf::RenderWindow& window) {
		sf::Vector2f spriteSize = (sf::Vector2f)sprite.getTexture()->getSize();
		sf::Vector2f spritePos = sprite.getPosition() + spriteSize;
		sf::Vector2f windowSize = (sf::Vector2f)window.getSize() + spriteSize;

		return (spritePos.x > 0.0f) && (spritePos.x < windowSize.x)
			&& (spritePos.y > 0.0f) && (spritePos.y < windowSize.y);
	}

	bool isColliding(const sf::Sprite& a, const sf::Sprite& b) {
		sf::Vector2f thisPos = a.getPosition();
		sf::Vector2f thisSize = (
			sf::Vector2f
			)(
			a.getTexture()
			->getSize()
		);

		sf::Vector2f targetPos = b.getPosition() - thisSize;
		sf::Vector2f targetSize = (sf::Vector2f)(b.getTexture()->getSize()) + thisSize;

		return (thisPos.x > targetPos.x) && (thisPos.x < targetPos.x + targetSize.x)
			&& (thisPos.y > targetPos.y) && (thisPos.y < targetPos.y + targetSize.y);
	}

}