#pragma once


class Timer {

	double startTime = 0.0;
	double currentTime = 0.0;
	float deltaTime = 0.0f;

	std::chrono::steady_clock::duration lastFrameTime;
	std::chrono::steady_clock::duration currentFrameTime;

	std::chrono::steady_clock::duration getTime(void* _) {
		return std::chrono::high_resolution_clock::now().time_since_epoch();
	}

	double getTime() {
		return double(std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::high_resolution_clock::now().time_since_epoch()).count()) / 1000000.0;
	}

public:

	Timer() { startTime = currentTime = float(getTime()); }

	const float& getDeltaTime() { return deltaTime; };

	void update() {
		currentFrameTime = getTime(nullptr);
		deltaTime = float((currentFrameTime - lastFrameTime).count() / 1000000000.0);
		lastFrameTime = currentFrameTime;
	}

};