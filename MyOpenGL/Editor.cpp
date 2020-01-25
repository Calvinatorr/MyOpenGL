#include "Editor.h"


void GLFW_ErrorCallback(int Error, const char * Description)
{
	Log::PrintError("GLFW Error " + std::to_string(Error) + "+: " + std::string(Description) + "\n");
}



EditorWindow::EditorWindow(const bool& bOpen)
{
#if WITH_EDITOR
	if (bOpen)
	{
		Editor::RegisterWindow(this);
	}
#endif
}

EditorWindow::~EditorWindow()
{
#if WITH_EDITOR
	
#endif
}

void EditorWindow::OpenWindow()
{
#if WITH_EDITOR
	Editor::RegisterWindow(this);
#endif
}

void EditorWindow::CloseWindow()
{
#if WITH_EDITOR
	Editor::UnregisterWindow(this);
#endif
}

void EditorWindow::ToggleWindow()
{
#if WITH_EDITOR
	if (Editor::editorWindows.find(this) != Editor::editorWindows.end())
	{
		Editor::UnregisterWindow(this);
	}
	else
	{
		Editor::RegisterWindow(this);
	}
#endif
}


bool EditorWindow::IsVisible()
{
	return bIsVisible;
}


void EditorWindow::SetVisible(bool NewVisibility)
{
	bIsVisible = NewVisibility;
}


void EditorWindow::ToggleVisibility()
{
	bIsVisible = !bIsVisible;
}


std::set<EditorWindow*> Editor::editorWindows = std::set<EditorWindow*>();
std::set<_DrawableBase*> Editor::drawables = std::set<_DrawableBase*>();


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
	for (auto& x : editorWindows)
	{
		if (x != nullptr)
		{
			if (x->IsVisible())
			{
				x->DrawWindow();
			}
		}
	}
#endif
}


void Editor::Cleanup()
{
#if WITH_EDITOR
	for (auto& x : editorWindows)
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

void Editor::RegisterWindow(EditorWindow * EditorObject)
{
#if WITH_EDITOR
	std::stringstream ss;
	editorWindows.insert(EditorObject);
	ss << "Registered editor GUI drawable with editor '" << EditorObject << "'";
	Log::PrintInfo(ss.str());
#endif
}

void Editor::RegisterDrawable(_DrawableBase * EditorObject)
{
#if WITH_EDITOR
	std::stringstream ss;
	drawables.insert(EditorObject);
	ss << "Registered drawable with editor '" << EditorObject << "'";
	Log::PrintInfo(ss.str());
#endif
}

void Editor::UnregisterWindow(EditorWindow * EditorObject)
{
#if WITH_EDITOR
	editorWindows.erase(EditorObject);
#endif
}

void Editor::UnregisterDrawable(_DrawableBase * EditorObject)
{
#if WITH_EDITOR
	drawables.erase(EditorObject);
#endif
}
