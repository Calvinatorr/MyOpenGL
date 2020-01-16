#include "GUI.h"


/*
std::vector<Widget> Widget::allWidgets = std::vector<Widget>();


Widget::Widget()
{
}

Widget::~Widget()
{
}

void Widget::DrawLayout()
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
*/

void GLFW_ErrorCallback(int Error, const char * Description)
{
	Log::PrintError("GLFW Error " + std::to_string(Error) + "+: " + std::string(Description) + "\n");
}