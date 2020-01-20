#pragma once

#include "SceneComponent.h"
#include "Mesh.h"

// Container for static mesh in the scene
class StaticMeshComponent : public SceneComponent
{
public:

	// Constructors & Destructors
	StaticMeshComponent();
	StaticMeshComponent(Mesh* StaticMesh);
	~StaticMeshComponent();


	// Properties
	Mesh* staticMesh;


	// Events
	virtual void Draw() override;
	virtual void DrawGUI() override;
};

