#pragma once

#include "SceneObject.h"
#include "StaticMeshComponent.h"


class StaticMeshObject : public SceneObject
{
public:

	// Constructors & destructors
	StaticMeshObject();
	~StaticMeshObject();


	// Components
	StaticMeshComponent staticMeshComponent;


	// Interface implementations
	void Construct() override;
	void DrawGUI() override;
};

