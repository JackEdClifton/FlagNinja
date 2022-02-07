
#include "pch.h"

#include "timer.h"


std::chrono::steady_clock::duration Timer::getTime(void* _) {
	return std::chrono::high_resolution_clock::now().time_since_epoch();
}

double Timer::getTime() {
	return double(std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 1000000.0;
}


Timer::Timer() { startTime = currentTime = float(getTime()); }

const float& Timer::getDeltaTime() { return deltaTime; };

void Timer::update() {
	currentFrameTime = getTime(nullptr);
	deltaTime = float((currentFrameTime - lastFrameTime).count() / 1000000000.0);
	lastFrameTime = currentFrameTime;
}
