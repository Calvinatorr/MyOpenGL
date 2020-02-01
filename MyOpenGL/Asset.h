#pragma once


#include "Object.h"
#include "Editor.h"

#include <map>
#include <set>
#include <filesystem>


class AssetManager;
class Asset;


/* Query assets from content browser */
class AssetManager
{
private:

#define ASSET_MAPPING_TYPE std::set<Asset*>
	static ASSET_MAPPING_TYPE assets;

#if WITH_EDITOR
	static bool bIsOpen;
	static uint ID;
#endif

public:

	friend class Asset;


	static void Register(Asset* NewAsset);
	static Asset* const GetAsset(const uint& Index);

	/* Get set of Asset* via input type */
	template <typename T>
	static std::set<T*> GetAssetsByType();

	/* Get asset mappings */
	static const ASSET_MAPPING_TYPE& GetAssetMap();

	/* Draws a context menu for selecting assets */
	template <typename T>
	static T* DrawAssetBrowserContextMenu(T** AssetToEdit);

	/* Resets state of asset manager */
	static void ResetState();
};



template<typename T>
inline std::set<T*> AssetManager::GetAssetsByType()
{
	std::set<T*> result;

	for (auto it = assets.begin(); it != assets.end(); ++it)
	{
		T* casted = dynamic_cast<T*>(*it);

		if (casted != nullptr )
		{
			result.emplace(&(*casted));
		}
	}

	return result;
}


template<typename T>
inline T* AssetManager::DrawAssetBrowserContextMenu(T** AssetToEdit)
{
#if WITH_EDITOR
	// Don't draw if we're already open
	if (bIsOpen)
		return nullptr;

	T* selected = nullptr;

	// Draw menu
	if (ImGui::BeginPopupContextItem(("Asset Browser##" + std::to_string(++ID)).c_str()))
	{
		// Set singleton asset manager context menu as open so we can't open multiple menus on the next calls to this method within the update loop
		bIsOpen = true;

		std::set<T*> assets = AssetManager::GetAssetsByType<T>();
		
		Asset* asset = dynamic_cast<Asset*>(*AssetToEdit);
		if (asset != nullptr)
		{
			ImGui::Text(asset->GetDisplayName().c_str());
			ImGui::Separator();
			for (auto& a : assets)
			{
				if (a != nullptr)
				{
					if (ImGui::Selectable((a->GetDisplayName() + "##").c_str()))
					{
						ImGui::CloseCurrentPopup();
						selected = a;
					}

					ImGui::SetNextItemWidth(-1);
				}
			}
		}


		ImGui::EndPopup();
	}

	// If we clicked on the asset in the browser, then redirect the incoming pointer
	if (selected != nullptr)
	{
		*AssetToEdit = selected;
	}

	return selected;
#endif
}



class Asset : public Object, public EditorWindow
{
public:


	Asset();
	~Asset();
};