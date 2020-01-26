#pragma once


#include "Object.h"
#include "Editor.h"

#include <map>
#include <set>


class Asset : public Object, public EditorWindow
{
public:


	Asset();
	~Asset();
};


/* Query assets from content browser */
class AssetManager
{
private:
	static std::set<Asset*> assets;

public:

	static void Register(Asset* NewAsset);
	static Asset* const GetAsset(const std::string& AssetName);
	
};