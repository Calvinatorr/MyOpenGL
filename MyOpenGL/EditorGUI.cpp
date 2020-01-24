#include "EditorGUI.h"



EditorGrid::EditorGrid()
	: StaticMeshComponent()
{
	// ===================================== MESH ============================================

	staticMesh = new StaticMesh();
	if (staticMesh == nullptr)
		assert("Failed to create static mesh in EditorGrid");

	auto& meshSections = staticMesh->GetMeshSections_Modifiable();
	meshSections.push_back(MeshSection());
	MeshSection& plane = meshSections.back();


	// Vertices
	plane.AddVertex(Vertex({ 10.0f,  0.0f, -10.0f },	{ 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f })); // Front - Top right
	plane.AddVertex(Vertex({ 10.0f, -0.0f,  10.0f },	{ 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })); // Front - Bottom right
	plane.AddVertex(Vertex({ -10.0f, -0.0f,  10.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f })); // Front - Bottom left
	plane.AddVertex(Vertex({ -10.0f,  0.0f, -10.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f })); // Front - Top left

	// Indices
	plane.indices = {
		0, 1, 3,
		1, 2, 3
	};
	plane.Construct();


	// ===================================== SHADER ============================================

	gridShader.Compile(SHADER_PATH + "EditorGrid");
	gridMaterial = Material(&gridShader);
	plane.SetMaterial(&gridMaterial);


	// ===================================== REGISTER ===========================================

	// Register self with Editor::Widgets
	Editor::RegisterDrawable(this);
}
