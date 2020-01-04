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
	std::string name = "", displayName = "";
	GLint ID = -1;

public:
	Object();
	~Object();


	// Methods
	virtual json Serialize();
	virtual void Deserialize(const json& Data);


	// Getters
	GLint GetID() const;
	GLboolean IsValid() const;
	// Get unique name
	std::string GetName();
	// Get display name
	std::string GetDisplayName();
	// Get name of class
	virtual std::string GetClassName();
};

