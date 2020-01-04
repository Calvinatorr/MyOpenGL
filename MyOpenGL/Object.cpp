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

std::string Object::GetName()
{
	return name;
}

std::string Object::GetDisplayName()
{
	return displayName;
}

std::string Object::GetClassName()
{
	//return "Object";
	return typeid(this).name();
}
