#include "GUI.h"


void GLFW_ErrorCallback(int Error, const char * Description)
{
	Log::PrintError("GLFW Error " + std::to_string(Error) + "+: " + std::string(Description) + "\n");
}



std::vector<Widget*> Widget::all = std::vector<Widget*>();


Widget::Widget()
{
	Widget::all.push_back(this);
}

Widget::~Widget()
{
	auto found = std::find(Widget::all.begin(), Widget::all.end(), this);
	Widget::all.erase(found);
}

void Widget::Layout()
{
}

bool Widget::IsVisible()
{
	return bIsVisible;
}

void Widget::SetVisible(bool NewVisibility)
{
	bIsVisible = NewVisibility;
}

void Widget::ToggleActive()
{
	bIsActive = !bIsActive;
}

void Widget::Draw()
{
	for (auto t = Widget::all.begin(); t != Widget::all.end(); ++t)
	{
		if (*t != nullptr && (*t)->bIsActive)
		{
			(*t)->Layout();
		}
	}
}

