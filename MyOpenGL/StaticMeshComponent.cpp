#include "StaticMeshComponent.h"



StaticMeshComponent::StaticMeshComponent()
{
	SetDisplayName("StaticMeshComponent");
}

StaticMeshComponent::StaticMeshComponent(StaticMesh * StaticMesh)
{
	SetStaticMesh(StaticMesh);
	StaticMeshComponent();
}


StaticMeshComponent::~StaticMeshComponent()
{
	//Cleanup(); // Don't do this - it destroys our static mesh when this goes out of scope?
}

void StaticMeshComponent::SetStaticMesh(StaticMesh* NewStaticMesh)
{
	staticMesh = NewStaticMesh;

	materialOverrides.resize(staticMesh->GetMaterials().size()); // Resize overrides array
	uint index = 0;
	for (auto& m : materialOverrides)
	{
		if (m == nullptr)
		{
			materialOverrides[index] = NewStaticMesh->GetMaterials()[index];
		}
		++index;
	}
}

StaticMesh * StaticMeshComponent::GetStaticMesh()
{
	return staticMesh;
}

void StaticMeshComponent::SetMaterialOverride(const uint & Index, Material * NewMaterial)
{
	if (Index < materialOverrides.size())
	{
		materialOverrides[Index] = NewMaterial;
	}
}

Material * StaticMeshComponent::GetMaterialOverride(const uint& Index) const
{
	if (Index < materialOverrides.size())
	{
		return materialOverrides[Index];
	}
	else
	{
		return nullptr;
	}
}

std::vector<Material*>& StaticMeshComponent::GetMaterialOverrides()
{
	return materialOverrides;
}

void StaticMeshComponent::Draw()
{
	// Render our static mesh
	if (staticMesh != nullptr)
		staticMesh->Draw(GetWorldTransformMatrix(), materialOverrides);
}

void StaticMeshComponent::Cleanup()
{
	if (staticMesh != nullptr)
		staticMesh->Cleanup();
	//delete(staticMesh);
}

void StaticMeshComponent::DrawSceneComponentDetails()
{
	staticMesh->DrawDetails();
	StaticMesh* cached = staticMesh;
	AssetManager::DrawAssetBrowserContextMenu(&staticMesh);
	// Trigger material override reordering
	if (cached != staticMesh)
	{
		SetStaticMesh(staticMesh);
	}


	// Materials
	if (Editor::DrawPanel("Material Overrides (" + std::to_string(materialOverrides.size()) + ")"))
	{
		for (auto& m : materialOverrides)
		{
			if (m != nullptr)
			{
				m->DrawDetails();
				AssetManager::DrawAssetBrowserContextMenu(&m);
			}
			else
			{
				ImGui::Selectable("NULL");
				AssetManager::DrawAssetBrowserContextMenu<Material>(&m);
			}
		}

		Editor::EndPanel();
	}

	// Rendering
	if (Editor::DrawPanel("Rendering"))
	{
		ImGui::Checkbox("Visible", &bIsVisible);

		Editor::EndPanel();
	}
}
