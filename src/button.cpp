
#include "pch.h"
#include "button.h"
#include "colour.h"


void Button::setColour(const sf::Color& primaryColour, const sf::Color& secondaryColour) {
	this->primaryColour = primaryColour;
	this->secondaryColour = secondaryColour;

	background.setFillColor(this->secondaryColour);
	text.setFillColor(Colour::Text::Default);
}

void Button::setPosition(const float& x, const float& y) {
	background.setPosition(x, y);

	const sf::Vector2f textSize = { text.getGlobalBounds().width , text.getGlobalBounds().height };
	sf::Vector2f s = (background.getSize() - textSize) / 2.0f;
	text.setPosition(x + s.x, y + s.y);
}

void Button::setPosition(const sf::Vector2f& pos) {
	setPosition(pos.x, pos.y);
}

void Button::draw(sf::RenderWindow& window) {
	window.draw(background);
	window.draw(text);
}

void Button::active() {
	if (!isActive) {
		isActive = true;
		background.move(-10.0f, -10.0f);
		background.setSize(background.getSize() + sf::Vector2f(20.0f, 20.0f));
		background.setFillColor(primaryColour);
	}
}

void Button::inactive() {
	if (isActive) {
		isActive = false;
		background.move(+10.0f, +10.0f);
		background.setSize(background.getSize() - sf::Vector2f(20.0f, 20.0f));
		background.setFillColor(secondaryColour);
	}
}



void HighScoreButton::setPosition(const float& x, const float& y) {
	Button::setPosition(x, y);
	highScoreText.setPosition(x + 20.0f, y + 10.0f);
}

void HighScoreButton::draw(sf::RenderWindow& window) {
	Button::draw(window);
	window.draw(highScoreText);
}
