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
#include <algorithm>



void GLFW_ErrorCallback(int Error, const char* Description);


class _EditorDrawableGUIBase
{
protected:
	static const ImGuiTreeNodeFlags panelFlags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanFullWidth;

public:
	virtual void DrawGUI() {};
};


/* Editor widget automaticlaly registers with the Editor from its constructor */
class EditorWidget : public _EditorDrawableGUIBase, public _ObjectBase
{
private:
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
};




class Object;

class Editor
{
public:
	static std::vector<_EditorDrawableGUIBase*> editorGUIDrawables;
	static std::vector<_DrawableBase*> drawables;


	static void Draw();
	static void DrawWidgets();
	static void Cleanup();
	static void RegisterEditorGUIDrawable(_EditorDrawableGUIBase* EditorObject);
	static void RegisterDrawable(_DrawableBase* EditorObject);
};