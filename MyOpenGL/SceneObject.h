#pragma once


#include "Serialization.h"
#include "SceneComponent.h"
#include "Object.h"

#include <iostream>
#include <vector>
#include <set>



class SceneComponent; // Forward declaration to avoid compiler screaming at me


// Contain scene components and top-level logic for handling of components
class SceneObject : public Object
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
	// Serialize object for loading to & from disk
	json Serialize() override;
	void Deserialize(const json& Data) override;


	// Getters
	// Return all scene components belonging to this object
	std::set<SceneComponent*>& GetAllSceneComponents();
};

