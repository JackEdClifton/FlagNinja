#pragma once

#include <Windows.h>
#include "entity.h"


class Player : public Entity {

	bool controlsActive = true;
	
public:

	// constructor
	Player(const char* texturePath, float xPos = 0.0f, float yPos = 0.0f) : Entity(texturePath, xPos, yPos) {}

	// handle user input
	void handleInput(float deltaTime) {
		
		// reset position, for debugging purposes
		if (GetAsyncKeyState('Q')) pos = { 0.0f, 0.0f };
		
		// x axis input
		if (GetAsyncKeyState('A')) { vel.x = -maxVel; }
		if (GetAsyncKeyState('D')) { vel.x = maxVel; }

		// jump input
		if ((GetAsyncKeyState('W') || GetAsyncKeyState(' ')))
			vel.y = jumpVel;

		// drag & gravity
		vel.x *= drag;
		vel.y += gravity * deltaTime;
	}


};