#pragma once

#include "Camera.h"
#include "Window.h"

#include <algorithm>

class EditorCamera : public Camera
{
private:
	static EditorCamera* defaultCamera;

public:

	// Properties
	glm::vec2 cursorPosition;
	glm::vec2 sensitivity = glm::vec2(50.0f, 50.0f);
	float speed = 5.0f, shiftMultiplier = 2.5f;


	// Constructors & destructors
	EditorCamera();
	~EditorCamera();


	// Methods
	void Update		(const double& DeltaTime);
	void UpdateMouse(const double& DeltaTime, const glm::vec2& CursorPosition);
	void UpdateFOV	(const double& DeltaTime, const glm::vec2& ScrollOffset);
	void Reset();


	// Static methods (singleton)
	static void CleanupDefaultCamera();
	static EditorCamera* GetDefaultCamera();
};