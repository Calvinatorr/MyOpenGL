#pragma once

#include "SceneComponent.h"

#include <vector>
#include <set>

class SceneComponent; // Forward declaration to avoid compiler screaming at me

// Contain scene components and top-level logic for handling of components
class SceneObject
{
private:
	std::set<SceneComponent*> sceneComponents;

protected:
	// Register newly created scene component with internal set
	void RegisterSceneComponent(SceneComponent* NewComponent);

public:

	// Constructors & Destructors
	SceneObject();
	~SceneObject();


	// Properties
	Transform transform;


	// Virtual events
	// Construct object before use
	virtual void Construct();
	// Draw all renderable scene components
	virtual void Draw();
	// Update all scene components
	virtual void Update();
	// Destroy all scene components & perform any cleanup necessary, invoked from scene manager
	virtual void Destroy();


	// Getters
	// Return all scene components belonging to this object
	std::set<SceneComponent*>& GetAllSceneComponents();
};

