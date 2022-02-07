#pragma once

#include "pch.h"
#include <Windows.h>

#include "platform.h"
#include "entity.h"
#include "gun.h"

#include "player.h"


// getter
const Gun& Player::getGun() { return gun; }

// constructor
Player::Player(float xPos, float yPos, std::string texturePath) : Entity(xPos, yPos, texturePath) {}

// handle user input
void Player::handleInput(const float deltaTime, const sf::Vector2f& mousePos) {

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
