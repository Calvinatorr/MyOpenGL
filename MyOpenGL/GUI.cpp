#include "GUI.h"


void GLFW_ErrorCallback(int Error, const char * Description)
{
	Log::PrintError("GLFW Error " + std::to_string(Error) + "+: " + std::string(Description) + "\n");
}



std::vector<EditorWidget*> EditorWidget::all = std::vector<EditorWidget*>();


EditorWidget::EditorWidget()
{
#if WITH_EDITOR
	EditorWidget::all.push_back(this);
#endif
}

EditorWidget::~EditorWidget()
{
#if WITH_EDITOR
	auto found = std::find(EditorWidget::all.begin(), EditorWidget::all.end(), this);
	EditorWidget::all.erase(found);
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

void EditorWidget::DrawAll()
{
#if WITH_EDITOR
	for (auto t = EditorWidget::all.begin(); t != EditorWidget::all.end(); ++t)
	{
		if (*t != nullptr && (*t)->bIsActive)
		{
			(*t)->DrawGUI();
		}
	}
#endif
}

