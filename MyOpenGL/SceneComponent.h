#pragma once

#include "Transform.h"
#include "Object.h"
#include "SceneObject.h"

#include "GUI.h"


class Object;
class SceneObject; // Forward declaration to avoid compiler screaming at me

// Abstract component which is added to a SceneObject
class SceneComponent : public Object, public EditorDrawableGUI
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
	virtual void Draw();
	virtual void Update();
	virtual void Destroy();
	virtual void DrawGUI() override;
};

