#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>


#include "Serialization.h"


class _SerializableBase;
class _ObjectBase;
class _DrawableBase;
class _BindableBase;
class Object;



/* Abstract base for methods */
class _ObjectBase
{
public:
	// Methods
	virtual void Construct();
	virtual void Cleanup();
	virtual void Update();
};


/* Abstract base for drawable objects */
class _DrawableBase
{
public:
	virtual void Draw();

	bool bIsVisible = true;
};


/* Abstract base for bindable objects */
class _BindableBase
{
public:
	virtual void Bind();
	virtual void Unbind();
};

#ifndef DEFAULT_DISPLAY_NAME
#define DEFAULT_DISPLAY_NAME std::string("UNTITLED")
#endif

/* Abstract base for generic object */
class Object : public _ObjectBase //, public _SerializableBase
{
protected:
	std::string name = "", displayName = DEFAULT_DISPLAY_NAME;
	GLint ID = -1;

public:
	Object();
	~Object();


	// Serialisation
	virtual json Serialize();
	virtual void Deserialize(const json& Data);


	// Getters
	GLint GetID() const;
	GLboolean IsValid() const;
	/* Get unique name */
	std::string GetName() const;
	/* Get display name */
	std::string GetDisplayName() const;
	/* Get name of class */
	std::string GetClassName() const;


	// Setters
	/* Set display name */
	void SetDisplayName(const std::string& NewDisplayName);


	// Methods
	virtual void Construct() override;
	virtual void Cleanup() override;
	virtual void Update() override;
};

