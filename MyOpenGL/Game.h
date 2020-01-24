#pragma once


#include <glad/glad.h>  // Initialize with gladLoadGL()
#include <GLFW/glfw3.h> // Window & input handler



#include "Utility.h"

#include <iostream>



namespace Game
{
	namespace
	{
		double elapsedTime = 0.0f;
		double deltaTime = 0.0f;

		// FPS
		uint frameCount = 0;
		double startTime = 0.0f;
		double FPS = 0;
	}


	// Getters
	double GetElapsedTime();
	double GetDeltaTime();
	double GetFPS();

	// Methods
	void Initialize();
	void Update();
}