#include "EditorCamera.h"


EditorCamera* EditorCamera::defaultCamera = new EditorCamera();


EditorCamera::EditorCamera()
{
}


EditorCamera::~EditorCamera()
{
}

void EditorCamera::Update(const double& DeltaTime)
{
	GLFWwindow* window = Window::GetCurrent();
	if (window == nullptr)
		return;

	float fDeltaTime = float(DeltaTime);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glm::vec3 forward = GetForward();
		glm::vec3 right = GetRight();
		glm::vec3 up = GetUp();

		float s = glfwGetKey(Window::GetCurrent(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? speed * shiftMultiplier : speed;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			transform.position += forward * fDeltaTime * s;
		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			transform.position -= forward * fDeltaTime * s;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			transform.position -= right * fDeltaTime * s;
		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			transform.position += right * fDeltaTime * s;
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			transform.position += up * fDeltaTime * s;
		else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			transform.position -= up * fDeltaTime * s;
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) // Reset camera
	{
		pitch = 0.0f;
		yaw = 0.0f;
		roll = 0.0f;
		transform.position = glm::vec3(0.0f, 0.0f, 3.0f);
	}
}

void EditorCamera::UpdateMouse(const double& DeltaTime, const glm::vec2& CursorPosition)
{
	GLFWwindow* window = Window::GetCurrent();
	if (window == nullptr)
		return;

	static bool firstMouse = true;
	if (firstMouse)
	{
		cursorPosition.x = CursorPosition.x;
		cursorPosition.y = CursorPosition.y;
		firstMouse = false;
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2))
	{
		float xoffset = CursorPosition.x - cursorPosition.x;
		float yoffset = cursorPosition.y - CursorPosition.y;

		glm::vec2 s = sensitivity * float(DeltaTime);
		xoffset *= s.x;
		yoffset *= s.y;

		pitch = std::clamp(pitch + yoffset, -89.0f, 89.0f); // Pitch
		yaw -= xoffset; // Yaw
	}

	cursorPosition = CursorPosition;
}

void EditorCamera::UpdateFOV(const double& DeltaTime, const glm::vec2& ScrollOffset)
{
	fieldOfView = std::clamp((fieldOfView - ScrollOffset.y * 2.5f), 1.0f, 90.0f);
}


void EditorCamera::CleanupDefaultCamera()
{
	delete(defaultCamera);
}

EditorCamera* EditorCamera::GetDefaultCamera()
{
	return defaultCamera;
}
