#pragma once

#include <Windows.h>
#include "entity.h"
#include "gun.h"

class Player : public Entity {
	Gun gun = Gun("./assets/guns/pistol.psd");
	
public:

	// getter
	const Gun& getGun() { return gun; }
	void setP() { pos = { 0.0f, 0.0f }; }

	// constructor
	Player(const char* texturePath, float xPos = 0.0f, float yPos = 0.0f) : Entity(texturePath, xPos, yPos) {}

	// handle user input
	void handleInput(const float deltaTime, const sf::Vector2f& mousePos) {

		// x axis input
		if (GetAsyncKeyState('A')) { vel.x = -maxVel; }
		if (GetAsyncKeyState('D')) { vel.x = maxVel; }
		if (GetAsyncKeyState('S')) { vel.y = maxVel; }

		// jump input
		if ((GetAsyncKeyState('W') || GetAsyncKeyState(' ')) && jumps && !jumpCooldown) {
			vel.y = jumpVel;
			jumps -= 1;
			jumpCooldown = 0.2f;
		}

		// drag & gravity
		vel.x *= drag;
		vel.y += (vel.y < 0.0f ? 0.5f : 1.0f) * gravity * deltaTime;

		// weapon
		gun.setPosition(pos + size / 2.0f);
		gun.aimWeapon(mousePos);
	}

};