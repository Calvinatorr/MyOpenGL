#include "EditorCamera.h"



EditorCamera::EditorCamera()
{
}


EditorCamera::~EditorCamera()
{
}

void EditorCamera::Update(const float& DeltaTime)
{
	GLFWwindow* window = Window::GetCurrent();
	if (window == nullptr)
		return;

	glm::vec3 forward = GetForward();
	glm::vec3 right = GetRight();
	glm::vec3 up = GetUp();

	float s = glfwGetKey(Window::GetCurrent(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? speed * shiftMultiplier : speed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		transform.position += forward * DeltaTime * s;
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		transform.position -= forward *DeltaTime * s;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		transform.position -= right * DeltaTime * s;
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		transform.position += right * DeltaTime * s;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		transform.position += up * DeltaTime * s;
	else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		transform.position -= up *DeltaTime * s;

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) // Reset camera
	{
		pitch = 0.0f;
		yaw = 0.0f;
		roll = 0.0f;
		transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
	}
}

void EditorCamera::UpdateMouse(const float & DeltaTime, const glm::vec2& CursorPosition)
{
	float xoffset = CursorPosition.x - cursorPosition.x;
	float yoffset = cursorPosition.y - CursorPosition.y;
	cursorPosition = CursorPosition;

	glm::vec2 s = sensitivity * DeltaTime;
	xoffset *= s.x;
	yoffset *= s.y;

	pitch = std::clamp(pitch + yoffset, -89.0f, 89.0f); // Pitch
	yaw -= xoffset; // Yaw
}

void EditorCamera::UpdateFOV(const float& DeltaTime, const glm::vec2& ScrollOffset)
{
	fieldOfView = std::clamp((fieldOfView - ScrollOffset.y * 2.5f), 1.0f, 90.0f);
}
