#include "StaticMeshComponent.h"



StaticMeshComponent::StaticMeshComponent()
{
}

StaticMeshComponent::StaticMeshComponent(StaticMesh * StaticMesh)
{
	this->staticMesh = StaticMesh;
}


StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::Draw()
{
	// Render our static mesh
	if (staticMesh != nullptr)
		staticMesh->Draw(GetWorldTransformMatrix());
}

void StaticMeshComponent::DrawGUI()
{
	SceneComponent::DrawGUI();

	if (ImGui::TreeNodeEx("Static Mesh", panelFlags))
	{
		ImGui::LabelText("Static Mesh Asset", staticMesh->GetDisplayName().c_str());

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::TreeNodeEx("Materials", flags))
		{
			// We will change this when multiple materials are functional
			ImGui::Text("0");
			ImGui::SameLine();
			if (ImGui::TreeNodeEx(staticMesh->material->GetDisplayName().c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
			{
				
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}
