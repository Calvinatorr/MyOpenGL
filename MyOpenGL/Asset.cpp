#include "Asset.h"


ASSET_MAPPING_TYPE AssetManager::assets = ASSET_MAPPING_TYPE();

bool AssetManager::bIsOpen = false;
uint AssetManager::ID = 0;


void AssetManager::Register(Asset* NewAsset)
{
	if (NewAsset != nullptr)
	{
		assets.insert( NewAsset );
	}
}

Asset * const AssetManager::GetAsset(const uint& Index)
{
	if (0 <= Index && Index < assets.size())
	{
		return *std::next(assets.begin(), Index);
	}
	else
	{
		return nullptr;
	}
}


const ASSET_MAPPING_TYPE & AssetManager::GetAssetMap()
{
	return assets;
}

void AssetManager::ResetState()
{
#if WITH_EDITOR
	AssetManager::bIsOpen = false;
	AssetManager::ID = 0;
#endif
}




Asset::Asset()
	: EditorWindow(false)
{
}


Asset::~Asset()
{
}
