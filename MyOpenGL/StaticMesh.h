#pragma once

#include "Utility.h"
#include "Object.h"
#include "MeshSection.h"

#include <algorithm>
#include <iterator>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



class StaticMesh : public Object
{
private:

	friend class MeshSection;

	// OBJ parser methods
	std::vector<std::string> ConvertToTokens(const std::string& Line, const char& Deliminator = ' ');
	int FindToken(const std::vector<std::string>& Tokens, const std::string& Snippet);
	std::string CleanupToken(std::string Token);
	float FloatToken(std::string Token);
	int IntToken(std::string Token);
	glm::vec3 Vec3Token(const std::vector<std::string>& Tokens);
	glm::vec2 Vec2Token(const std::vector<std::string>& Tokens);
	std::vector<int> DecodeIndicesFromToken(const std::string& Token);

	/* Process aiMesh from assimp */
	void ProcessMeshSection(MeshSection* MeshSection, aiMesh* Mesh, const aiScene* Scene);
	/* Process aiNode from assimp */
	void ProcessNode(aiNode* Node, const aiScene* Scene);

	glm::vec3 minBounds, maxBounds;

	std::vector<MeshSection> meshSections;

protected:

	MeshSection::DrawMode drawMode = MeshSection::DrawMode::DrawElements;

	/* Calculate bounds of static mesh across all mesh sections */
	void CalculateBounds();
	void CalculateMetaData();


public:

	// Properties
	std::string source;
	

	// Constructors & destructors
	StaticMesh();
	~StaticMesh();


	// Methods
	/* DEPRECATED */
	void LoadMeshObj(const std::string& File);
	/* Implements assimp - http://www.assimp.org/ */
	bool LoadMeshFromDisk(const std::string& Filename);
	/* Reimports from disk */
	bool Reimport();

	/*Draws all mesh sections */
	void Draw(const glm::mat4& Transform = glm::mat4());
	/* Clears mesh sections */
	void Clear();
	/* Cleanup method */
	void Cleanup() override;

	/* Return list of materials */
	std::vector<Material*> GetMaterials() const;
	/* Set material at mesh section index */
	bool SetMaterial(const uint& Index, Material* NewMaterial);
	/* Return unmodified list of mesh sections */
	const std::vector<MeshSection>& GetMeshSections() const;

	/* Get min bounds */
	glm::vec3 GetMinBounds() const;
	/* Get maxbounds */
	glm::vec3 GetMaxBounds() const;
};

