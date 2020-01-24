#pragma once

#include <glad/glad.h>  // Hardware-specific loader
#include <GLFW/glfw3.h> // Window & input handler

// GLM Mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

class Window
{
public:
	// Enums defs
	enum WindowMode { WINDOW, BORDERLESS, FULLSCREEN };

private:
	static void Resized(GLFWwindow* Window, int Width, int Height);
	static Window* current;

	WindowMode windowMode = WindowMode::WINDOW;


public:
	

	// Properties
	GLFWwindow* window;

	Window();
	~Window();

	// Methods
	GLint Create(GLuint Width, GLuint Height, const GLchar* WindowName = "NewWindow", const bool& bMaximized = false, const WindowMode& NewWindowMode = WindowMode::WINDOW);
	void Maximize();
	void Restore();
	void Hide();
	void SwapBuffers();
	void Bind();

	
	// Getters
	glm::ivec2 GetSize() const;
	static GLFWwindow* GetCurrent();
	static Window* GetCurrentObject();
};

