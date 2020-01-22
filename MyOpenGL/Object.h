#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>

#include <string>
#include <sstream>


#include "Serialization.h"


class Object // Base class for all resources
{
protected:
	std::string name = "", displayName = "UNTITLED";
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
	virtual void Construct();
	virtual void Cleanup();
	virtual void Update();
};

