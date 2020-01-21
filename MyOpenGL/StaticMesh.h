#pragma once

#include "Utility.h"
#include "Object.h"
#include "MeshSection.h"

#include <algorithm>
#include <iterator>


class StaticMesh : public MeshSection
{
private:
	// OBJ parser methods
	std::vector<std::string> ConvertToTokens(const std::string& Line, const char& Deliminator = ' ');
	int FindToken(const std::vector<std::string>& Tokens, const std::string& Snippet);
	std::string CleanupToken(std::string Token);
	float FloatToken(std::string Token);
	int IntToken(std::string Token);
	glm::vec3 Vec3Token(const std::vector<std::string>& Tokens);
	glm::vec2 Vec2Token(const std::vector<std::string>& Tokens);
	std::vector<int> DecodeIndicesFromToken(const std::string& Token);

public:

	// Properties
	std::string source;
	

	// Constructors & destructors
	StaticMesh();
	~StaticMesh();


	// Methods
	void LoadMeshObj(const std::string& File);
};

