#pragma once

#include "SceneComponent.h"
#include "StaticMesh.h"

// Container for static mesh in the scene
class StaticMeshComponent : public SceneComponent
{
public:

	// Constructors & Destructors
	StaticMeshComponent();
	StaticMeshComponent(StaticMesh* StaticMesh);
	~StaticMeshComponent();


	// Properties
	StaticMesh* staticMesh;


	// Events
	virtual void Draw() override;
	virtual void DrawGUI() override;
};

