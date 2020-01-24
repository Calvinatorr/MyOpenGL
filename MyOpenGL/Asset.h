#pragma once


#include "Object.h"

#include "Editor.h"



class Asset : public Object
{
public:


	Asset();
	~Asset();


	virtual void DrawEditorWindow();
};

