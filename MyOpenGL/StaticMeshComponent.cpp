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

void StaticMeshComponent::Cleanup()
{
	if (staticMesh != nullptr)
		staticMesh->Cleanup();
}

void StaticMeshComponent::DrawGUI()
{
	SceneComponent::DrawGUI();
	staticMesh->DrawGUI();
}
