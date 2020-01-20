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