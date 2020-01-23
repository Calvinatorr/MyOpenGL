#include "StaticMeshObject.h"



StaticMeshObject::StaticMeshObject()
{

}


StaticMeshObject::~StaticMeshObject()
{
}

void StaticMeshObject::Construct()
{
	RegisterSceneComponent(&staticMeshComponent);
	SceneObject::Construct();
}

void StaticMeshObject::DrawGUI()
{
	SceneObject::DrawGUI();

	if (staticMeshComponent.staticMesh != nullptr)
		staticMeshComponent.staticMesh->DrawGUI();
}
