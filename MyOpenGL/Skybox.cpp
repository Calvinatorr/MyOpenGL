#include "Skybox.h"


Shader Skybox::skyboxShader;


void Skybox::Construct()
{
	static bool bInitializedShader = false;
	if (!bInitializedShader)
	{
		skyboxShader.Import(SHADER_PATH + "Cubemap");
		bInitializedShader = true;
	}

	staticMeshComponent.SetStaticMesh(&Primitives::cube);
	material.shader = &skyboxShader;
	material.SetTextureParameter("EnvironmentMap", environmentMap);


	RegisterSceneComponent(&staticMeshComponent);
	SceneObject::Construct();
}

void Skybox::DrawDetails()
{
	SceneObject::DrawDetails();

	ImGui::NewLine();

	std::string textureName = environmentMap == nullptr ? "NULL" : (environmentMap->GetDisplayName() + "(Texture)");
	ImGui::Selectable(textureName.c_str());
	AssetManager::DrawAssetBrowserContextMenu(&environmentMap);
	/*
	if (staticMeshComponent.GetStaticMesh() != nullptr)
	{
		StaticMesh* tmp = staticMeshComponent.GetStaticMesh();
		tmp->DrawDetails();
		AssetManager::DrawAssetBrowserContextMenu(&tmp);
	}
	*/
}

void Skybox::SetEnvironmentMap(TextureCube * NewEnvironmentMap)
{
	environmentMap = (Texture*)(NewEnvironmentMap);
	material.SetTextureParameter("EnvironmentMap", environmentMap);
}
