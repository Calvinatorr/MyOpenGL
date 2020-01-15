#pragma once

// IMGUI - https://github.com/ocornut/imgui/
#include "imgui.h"
#include "imgui.cpp"
#include "imgui_demo.cpp"
#include "imgui_draw.cpp"
#include "imgui_widgets.cpp"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_glfw.cpp"
#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_opengl3.cpp"


#include "Utility.h"


void GLFW_ErrorCallback(int Error, const char* Description)
{
	Log::PrintError("GLFW Error " + std::to_string(Error) + "+: " + std::string(Description) + "\n");
}