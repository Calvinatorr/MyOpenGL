#pragma once

#include "Primitives.h"
#include "SceneObject.h"
#include "StaticMeshComponent.h"


class Skybox : public SceneObject
{
protected:

	static Shader skyboxShader;

	Material material;
	Texture* environmentMap;

public:
	
	// Components
	StaticMeshComponent staticMeshComponent;


	// Interface implementations
	void Construct() override;
	void DrawDetails() override;

	void SetEnvironmentMap(TextureCube* NewEnvironmentMap);
};

