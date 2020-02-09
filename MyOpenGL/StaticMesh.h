#pragma once

#include "Utility.h"
#include "Object.h"
#include "MeshSection.h"

#include "Editor.h"

#include <algorithm>
#include <iterator>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



class StaticMesh : public Asset, public _DrawableBase, public _EditorDetailsBase
{
private:

	friend class MeshSection;

	/* Process aiMesh from assimp */
	void ProcessMeshSection(MeshSection* MeshSection, aiMesh* Mesh, const aiScene* Scene);
	/* Process aiNode from assimp */
	void ProcessNode(aiNode* Node, const aiScene* Scene);

	glm::vec3 minBounds, maxBounds;


	class ImportSetting
	{
	public:
		std::string label;
		bool value = true;

		ImportSetting(const std::string& Label, const bool& Value = true)
			: label(Label), value(Value)
		{};
	};



protected:

	MeshSection::DrawMode drawMode = MeshSection::DrawMode::DrawElements;

	/* List of mesh sections */
	std::vector<MeshSection> meshSections;


	/* Calculate bounds of static mesh across all mesh sections */
	void CalculateBounds();
	void CalculateMetaData();

	std::map<aiPostProcessSteps, ImportSetting> importSettings = {
		{aiProcess_Triangulate,				{"Triangulate",					true}},
		{aiProcess_FlipUVs,					{"Flip UVs",					true}},
		{aiProcess_CalcTangentSpace,		{"Calculate Tangent Space",		true}},
		{aiProcess_OptimizeMeshes,			{"Optimize Meshes",				true}},
		{aiProcess_SplitLargeMeshes,		{"Split Large Meshes",			false}},
		{aiProcess_PreTransformVertices,	{"Pre-Transform Vertices",		true}},
		{aiProcess_Debone,					{"Debone",						false}}
	};


public:

	// Constructors & destructors
	StaticMesh();
	~StaticMesh();


	// Methods
	/* Implements assimp - http://www.assimp.org/ */
	bool Import(const std::string& Filename) override;
	/* Reimports from disk */
	bool Reimport();


	/* Draws all mesh sections */
	void Draw(const glm::mat4& Transform = glm::mat4(1.0f), const std::vector<Material*>& MaterialOverrides = std::vector<Material*>(), const bool& bBindMaterials = true, const bool& bBindDefaultShader = true);
	/* Draws all mesh sections */
	void Draw() override;
	/* Cleanup method */
	void Cleanup() override;
	/* Construct mesh - calls CalculateMetaData() */
	void Construct() override;


	/* Return list of materials */
	std::vector<Material*> GetMaterials() const;
	/* Set material at mesh section index */
	bool SetMaterial(const uint& Index, Material* NewMaterial);


	/* Return unmodifiable list of mesh sections */
	const std::vector<MeshSection>& GetMeshSections() const;
	/* Return mesh sections which you can then modify. Not recommended and potentially unsafe. */
	std::vector<MeshSection>& GetMeshSections_Modifiable();
	/* Clear mesh sections */
	void ClearMeshSections();


	/* Get min bounds */
	glm::vec3 GetMinBounds() const;
	/* Get maxbounds */
	glm::vec3 GetMaxBounds() const;



	// Interface implementations
	/* Draw editor GUI */
	virtual void DrawDetails() override;
	virtual void DrawWindow() override;
};

