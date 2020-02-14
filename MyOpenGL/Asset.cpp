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

bool Asset::Reimport()
{
	bool bSuccess = false;

	if (!source.empty())
	{
		bSuccess = Import(source);
	}

	return bSuccess;
}


void Asset::BeginAssetWindow()
{
	ImGui::Begin(GetAssetWindowTitle().c_str(), &bIsWindowVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

	// Menu bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();

			// Close window
			if (ImGui::MenuItem("Exit"))
			{
				SetWindowVisible(false);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::Button("Reimport"))
		{
			Reimport();
		}

		ImGui::EndMenuBar();
	}

	// Asset reimport
	ImGui::LabelText("Source", source.c_str());
	ImGui::SameLine();
	if (ImGui::Button("..."))
	{
		/*#if OS_Windows
					char filename[MAX_PATH] = "../";

					OPENFILENAME ofn;
					ZeroMemory(&filename, sizeof(filename));
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
					ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
					ofn.lpstrFile = filename;
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrTitle = "Select a File, yo!";
					ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

					if (GetOpenFileNameA(&ofn))
					{
						std::cout << "You chose the file \"" << filename << "\"\n";
					}
		#endif*/
	}
	ImGui::Separator();
	ImGui::NewLine();
}


void Asset::DrawWindow()
{
	BeginAssetWindow();
	DrawWindowContents();
	EndAssetWindow();
}

std::string Asset::GetAssetWindowTitle()
{
	return GetDisplayName() + " (" + GetClassNameA() + ")";
}

void Asset::EndAssetWindow()
{
	ImGui::End();
}
