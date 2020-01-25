#pragma once


#include "Object.h"

#include "Editor.h"



class Asset : public Object, public _EditorWindowBase
{
protected:
	bool bIsWindowActive = false;

public:


	Asset();
	~Asset();
};


class AssetManager
{

};