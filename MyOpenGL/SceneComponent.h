#pragma once

#include "Transform.h"
#include "Object.h"
#include "SceneObject.h"

#include "Editor.h"


class Object;
class SceneObject; // Forward declaration to avoid compiler screaming at me

// Abstract component which is added to a SceneObject
class SceneComponent : public Object, public _DrawableBase, public _EditorDetailsBase
{
public:

	// Constructors & destructors
	SceneComponent();
	~SceneComponent();


	// Properties
	Transform localTransform;
	SceneComponent* parent = nullptr;
	SceneObject* owner = nullptr;


	// Getters
	Transform GetWorldTransform() const; // Implement me, return local for now
	glm::mat4 GetWorldTransformMatrix() const;


	// Virtual events
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Cleanup() override;
	virtual void DrawSceneComponentDetails();
	virtual void DrawDetails() override;
};

