#pragma once


class Timer {

	double startTime = 0.0;
	double currentTime = 0.0;
	float deltaTime = 0.0f;

	std::chrono::steady_clock::duration lastFrameTime;
	std::chrono::steady_clock::duration currentFrameTime;

	std::chrono::steady_clock::duration getTime(void* _);

	double getTime();

public:

	Timer();
	const float& getDeltaTime();
	void update();

};