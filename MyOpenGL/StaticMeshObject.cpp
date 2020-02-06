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
	staticMeshComponent.DrawSceneComponentDetails();
	/*
	if (staticMeshComponent.GetStaticMesh() != nullptr)
	{
		StaticMesh* tmp = staticMeshComponent.GetStaticMesh();
		tmp->DrawDetails();
		AssetManager::DrawAssetBrowserContextMenu(&tmp);
	}
	*/
}
