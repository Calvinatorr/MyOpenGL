#pragma once


#include "Serialization.h"
#include "SceneComponent.h"
#include "Object.h"

#include "Editor.h"

#include <iostream>
#include <vector>
#include <set>



class SceneComponent; // Forward declaration to avoid compiler screaming at me


// Contain scene components and top-level logic for handling of components
class SceneObject : public Object, public _DrawableBase, public _EditorDetailsBase
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


	// Interface implementations
	// Construct object before use
	virtual void Construct() override;
	// Draw all renderable scene components
	virtual void Draw() override;
	// Update all scene components
	virtual void Update() override;
	// Destroy all scene components & perform any cleanup necessary, invoked from scene manager
	virtual void Destroy();
	/* Draw editor GUI */
	virtual void DrawDetails() override;


	// Serialize object for loading to & from disk
	json Serialize() override;
	void Deserialize(const json& Data) override;


	// Scene components
	/* Return all scene components belonging to this object */
	std::set<SceneComponent*>& GetAllSceneComponents();
	/* Add a scene component and register it */
	void AddSceneComponent(SceneComponent* NewSceneComponent);
};

