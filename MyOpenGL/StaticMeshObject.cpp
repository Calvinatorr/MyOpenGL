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

void StaticMeshObject::DrawDetails()
{
	SceneObject::DrawDetails();

	if (staticMeshComponent.staticMesh != nullptr)
		staticMeshComponent.staticMesh->DrawDetails();
}
