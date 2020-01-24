#include "Editor.h"


void GLFW_ErrorCallback(int Error, const char * Description)
{
	Log::PrintError("GLFW Error " + std::to_string(Error) + "+: " + std::string(Description) + "\n");
}



EditorWidget::EditorWidget()
{
#if WITH_EDITOR
	Editor::RegisterEditorGUIDrawable(this);
#endif
}

EditorWidget::~EditorWidget()
{
#if WITH_EDITOR
	
#endif
}


bool EditorWidget::IsVisible()
{
	return bIsVisible;
}


void EditorWidget::SetVisible(bool NewVisibility)
{
	bIsVisible = NewVisibility;
}


void EditorWidget::ToggleActive()
{
	bIsActive = !bIsActive;
}


std::vector<_EditorDrawableGUIBase*> Editor::editorGUIDrawables = std::vector<_EditorDrawableGUIBase*>();
std::vector<_DrawableBase*> Editor::drawables = std::vector<_DrawableBase*>();


void Editor::Draw()
{
#if WITH_EDITOR
	for (auto& x : drawables)
	{
		if (x != nullptr)
		{
			x->Draw();
		}
	}
#endif
}

void Editor::DrawWidgets()
{
#if WITH_EDITOR
	for (auto& x : editorGUIDrawables)
	{
		if (x != nullptr)
		{
			x->DrawGUI();
		}
	}
#endif
}


void Editor::Cleanup()
{
#if WITH_EDITOR
	for (auto& x : editorGUIDrawables)
	{
		if (x != nullptr)
		{
			auto c = dynamic_cast<_ObjectBase*>(x);
			if (c != nullptr)
			{
				c->Cleanup();
			}
		}
	}

	for (auto& x : drawables)
	{
		if (x != nullptr)
		{
			auto c = dynamic_cast<_ObjectBase*>(x);
			if (c != nullptr)
			{
				c->Cleanup();
			}
		}
	}
#endif
}

void Editor::RegisterEditorGUIDrawable(_EditorDrawableGUIBase * EditorObject)
{
#if WITH_EDITOR
	std::stringstream ss;
	editorGUIDrawables.push_back(EditorObject);
	ss << "Registered editor GUI drawable with editor '" << EditorObject << "'";
	Log::PrintInfo(ss.str());
#endif
}

void Editor::RegisterDrawable(_DrawableBase * EditorObject)
{
#if WITH_EDITOR
	std::stringstream ss;
	drawables.push_back(EditorObject);
	ss << "Registered drawable with editor '" << EditorObject << "'";
	Log::PrintInfo(ss.str());
#endif
}