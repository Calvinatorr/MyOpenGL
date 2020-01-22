#include "Object.h"



Object::Object()
{
}


Object::~Object()
{
}

json Object::Serialize()
{
	json jsonData = {
		{"metadata", {
			{"class", this->GetClassName()},
			{"name", GetName()},
			{"displayName", GetDisplayName()}
		}}
	};

	return jsonData;
}

void Object::Deserialize(const json & Data)
{
	json metadata = Data["metadata"];
	name = metadata["name"];
	displayName = metadata["displayName"];
}

GLint Object::GetID() const
{
	return ID;
}

GLboolean Object::IsValid() const
{
	return ID >= 0;
}

void Object::SetDisplayName(const std::string & NewDisplayName)
{
	displayName = NewDisplayName;
}

void Object::Construct()
{
}

void Object::Cleanup()
{
}

void Object::Update()
{
}

std::string Object::GetName() const
{
	return name;
}

std::string Object::GetDisplayName() const
{
	return displayName;
}

std::string Object::GetClassName() const
{
	std::string name = typeid(*this).name(); // COMPILER SPECIFIC

	// Strip "class " from string if inserted by the compiler 
	const std::string SUB_STRING = "class ";
	int foundPos = name.find(SUB_STRING);
	if (foundPos != std::string::npos)
		name.erase(name.begin() + foundPos, name.begin() + foundPos + SUB_STRING.length());

	return name;
}
