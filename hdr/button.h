#pragma once


struct Button {

	bool isActive = false;

	sf::RectangleShape background;
	sf::Text text;

	sf::Color primaryColour;
	sf::Color secondaryColour;

	void setColour(const sf::Color& primaryColour, const sf::Color& secondaryColour);
	void setPosition(const float& x, const float& y);
	void setPosition(const sf::Vector2f& pos);
	void draw(sf::RenderWindow& window);

	void active();
	void inactive();
};




struct HighScoreButton : public Button {
	sf::Text highScoreText;

	void setPosition(const float& x, const float& y);
	void draw(sf::RenderWindow& window);
};