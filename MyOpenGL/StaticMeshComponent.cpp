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
	//Cleanup(); // Don't do this - it destroys our static mesh when this goes out of scope?
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
	//delete(staticMesh);
}

void StaticMeshComponent::DrawGUI()
{
	SceneComponent::DrawGUI();
	staticMesh->DrawGUI();
}
