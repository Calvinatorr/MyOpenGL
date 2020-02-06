#pragma once

#include <glad/glad.h>  // Initialize with gladLoadGL()
#include <GLFW/glfw3.h> // Window & input handler


// IMGUI - https://github.com/ocornut/imgui/
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"


#include "Utility.h"
#include "Object.h"


#include <vector>
#include <set>
#include <algorithm>



void GLFW_ErrorCallback(int Error, const char* Description);


/* Derrive all editor functionality from here */
class _EditorBase
{
protected:
	static const ImGuiTreeNodeFlags panelFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;
};


/* Draw widget */
class _EditorWidgetBase : public _EditorBase
{
public:
	virtual void DrawGUI() {};
};


/* Draw details panel from here (Assets, Components, Objects) */
class _EditorDetailsBase : public _EditorBase
{
public:
	virtual void DrawDetails() {};
};


/* Draw windows from here (Assets)
class _EditorWindowBase : public _EditorBase
{
public:
	
};*/


/* Editor widget automaticlaly registers with the Editor from its constructor */
class EditorWindow : public _ObjectBase  //, public _EditorWindowBase
{
protected:
	bool bIsWindowVisible = true;

public:

	EditorWindow(const bool& bOpen = true);
	~EditorWindow();

	// Methods
	/* Registers with Editor */
	void OpenWindow();
	/* Unregisters window with Editor (closes) */
	void CloseWindow();
	/* Toggle window */
	void ToggleWindow();


	// Getters
	bool IsWindowVisible();


	// Setters
	void SetWindowVisible(bool NewVisibility);
	void ToggleWindowVisibility();


	// Implementable methods
	virtual void DrawWindow() {};
};


class Object;

class Editor
{
public:
	static std::set<EditorWindow*> editorWindows;
	static std::set<_DrawableBase*> drawables;


	static void Draw();
	static void DrawWidgets();
	static void Cleanup();
	static void RegisterWindow(EditorWindow* EditorObject);
	static void RegisterDrawable(_DrawableBase* EditorObject);
	static void UnregisterWindow(EditorWindow* EditorObject);
	static void UnregisterDrawable(_DrawableBase* EditorObject);

	/* Helper methods */
	static bool DrawPanel(const std::string& PanelTitle);
	static void EndPanel();
};