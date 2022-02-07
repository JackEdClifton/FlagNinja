
#include <iostream>
#include "SFML/Graphics.hpp"

// override output for cout
std::ostream& operator<<(std::ostream& stream, sf::Vector2f vector) {
	stream << "(" << vector.x << ", " << vector.y << ")";
	return stream;
}


// create devision operator
sf::Vector2f operator/(sf::Vector2f a, sf::Vector2f b) {
	a.x /= b.x;
	a.y /= b.y;
	return a;
}

// create multiplication operator
sf::Vector2f operator*(sf::Vector2f a, sf::Vector2f b) {
	a.x *= b.x;
	a.y *= b.y;
	return a;
}

sf::Vector2f operator*=(sf::Vector2f a, sf::Vector2f b) {
	a = a * b;
	return a;
}