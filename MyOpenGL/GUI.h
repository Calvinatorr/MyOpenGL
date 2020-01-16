#pragma once

#include <glad/glad.h>  // Initialize with gladLoadGL()
#include <GLFW/glfw3.h> // Window & input handler


// IMGUI - https://github.com/ocornut/imgui/
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include "Utility.h"


//#include <vector>
//#include <algorithm>



void GLFW_ErrorCallback(int Error, const char* Description);


/*
class Widget
{
private:
	static std::vector<Widget> allWidgets;
	bool bIsVisible = true;


public:

	Widget();
	~Widget();


	// Methods
	virtual void DrawLayout();
	
	// Getters
	bool IsVisible();

	// Setters
	void SetVisible(bool NewVisibility);
};
*/