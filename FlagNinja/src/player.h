#pragma once

#include <Windows.h>
#include "entity.h"


class Player : public Entity {

	
public:

	// constructor
	Player(const char* texturePath, float xPos = 0.0f, float yPos = 0.0f) : Entity(texturePath, xPos, yPos) {}

	// handle user input
	void handleInput(float deltaTime) {
		
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
	}


};