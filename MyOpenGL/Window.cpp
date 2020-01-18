#include "Window.h"

Window* Window::current = nullptr;

void Window::Resized(GLFWwindow * Window, int Width, int Height)
{
	// Make sure viewport matches new window dimensions when resized
	glfwGetFramebufferSize(Window, &Width, &Height);
	glViewport(0, 0, Width, Height);
}

Window::Window()
{
}

Window::~Window()
{
}

GLint Window::Create(GLuint Width, GLuint Height, const GLchar* WindowName, const bool& bMaximized, const WindowMode& NewWindowMode)
{
	GLFWmonitor* monitor = NULL;
	
	switch (NewWindowMode)
	{
		case WindowMode::WINDOW:
		{
			monitor = NULL;

			// Window hints
			if (bMaximized)
			{
				glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
				const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
				Width = mode->width;
				Height = mode->height;
			}

			break;
		}

		case WindowMode::BORDERLESS:
		{
			// This 
			glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE); // Maximize
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Hide window decorations
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			Width = mode->width;
			Height = mode->height;

			// This is crap - not truly what we think of as borderless fullscreen
			/*monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			Width	= mode->width;
			Height	= mode->height;*/

			break;
		}

		case WindowMode::FULLSCREEN:
		{
			monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
			break;
		}
	}

	glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

	window = glfwCreateWindow(Width, Height, WindowName, monitor, NULL);


	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1; // Error
	}
	glfwSetFramebufferSizeCallback(window, Window::Resized);


	Bind();

	return 1; // Success
}

void Window::Maximize()
{
	glfwMaximizeWindow(window);
}

void Window::Restore()
{
	glfwRestoreWindow(window);
}

void Window::Hide()
{
	glfwHideWindow(window);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window); // Swap the colour buffer and show to screen
}

void Window::Bind()
{
	Window::current = this;
	glfwMakeContextCurrent(window); // Set context to our new window
}

glm::ivec2 Window::GetSize() const
{
	GLint width, height;
	glfwGetWindowSize(window, &width, &height);
	return glm::ivec2(width, height);
}

GLFWwindow * Window::GetCurrent()
{
	return GetCurrentObject()->window;
}

Window * Window::GetCurrentObject()
{
	return Window::current;
}
