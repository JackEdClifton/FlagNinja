#pragma once

std::ostream& operator<<(std::ostream&, sf::Vector2f);

sf::Vector2f operator/(sf::Vector2f, sf::Vector2f);
sf::Vector2f operator*(sf::Vector2f, sf::Vector2f);
sf::Vector2f operator*=(sf::Vector2f, sf::Vector2f);