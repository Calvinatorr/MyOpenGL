#pragma once

#include <glad/glad.h>  // Initialize with gladLoadGL()
#include <GLFW/glfw3.h> // Window & input handler


// IMGUI - https://github.com/ocornut/imgui/
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"


#include "Utility.h"


#include <vector>
#include <algorithm>



void GLFW_ErrorCallback(int Error, const char* Description);


class EditorDrawableGUI
{
protected:
	static const ImGuiTreeNodeFlags panelFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;

public:
	virtual void DrawGUI() {};
};


class EditorWidget : public EditorDrawableGUI
{
private:
	static std::vector<EditorWidget*> all;
	bool bIsVisible = true;

protected:
	bool bIsActive = true;
	static const ImGuiTreeNodeFlags panelFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;

public:

	EditorWidget();
	~EditorWidget();


	// Getters
	bool IsVisible();

	// Setters
	void SetVisible(bool NewVisibility);
	void ToggleActive();

	// Static methods
	static void DrawAll();
};