
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