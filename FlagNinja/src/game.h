#pragma once

#include <chrono>

class Timer {

	float startTime = 0.0f;
	float currentTime = 0.0f;
	
	static float getTime() {
		return float(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 100000000.0f;
	}

public:
	float deltaTime = 0.0f;

	Timer() {
		startTime = getTime();
		currentTime = getTime();
	}


	void update() {
		currentTime = getTime();
		deltaTime = currentTime - startTime;
		startTime = currentTime;
	}

};

