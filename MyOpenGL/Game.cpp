#include "Game.h"

double Game::GetElapsedTime()
{
	return elapsedTime;
}

double Game::GetDeltaTime()
{
	return deltaTime;
}

double Game::GetFPS()
{
	return FPS;
}

void Game::Initialize()
{
	frameCount = 0;
	startTime = 0.0f;
	FPS = 0.0f;
}

void Game::Update()
{
	// Get current time
	double currentTime = glfwGetTime();
	frameCount++; // Increment num of frames which have passed

	// If a second has passed
	//if (currentTime - startTime >= 1.0)
	if (currentTime - startTime >= 0.25 && frameCount > 10)
	//if (currentTime - startTime >= .1 && frameCount > 10)
	{
		FPS = (double)frameCount / (currentTime - startTime);
		frameCount = 0; // Reset frame count
		startTime = currentTime;
	}

	deltaTime = currentTime - elapsedTime;
	elapsedTime = currentTime;
}
