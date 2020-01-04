#pragma once

#include <typeinfo>
#include <iostream>


// JSON serialization
#include <nlohmann/json.hpp>
using json = nlohmann::json;


// GLM Mathematics library
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm/gtx/string_cast.hpp"



// Forward declarations

// Serialize type to JSON data
template<typename T>
json SerializeValue(const T& Value);




// Serialize generic type to JSON data
template<typename T>
inline json SerializeValue(const T & Value)
{
	json jsonData;
	jsonData << Value;
	return jsonData;
}