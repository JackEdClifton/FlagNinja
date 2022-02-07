#pragma once


class Player : public Entity {
	Gun gun = Gun("./assets/guns/pistol.psd");
	
public:

	// getter
	const Gun& getGun();
	
	// constructor
	Player(float xPos, float yPos, std::string texturePath);

	// handle user input
	void handleInput(const float deltaTime, const sf::Vector2f& mousePos);

};