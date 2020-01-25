#pragma once


#include "Object.h"
#include "Editor.h"



class Asset : public Object, public EditorWindow
{
public:


	Asset();
	~Asset();
};