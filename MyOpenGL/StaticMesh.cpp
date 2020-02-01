#include "StaticMesh.h"

void StaticMesh::CalculateBounds()
{
	if (meshSections.size() <= 0)
		return;

	glm::vec3 min = glm::vec3(0.0f), max = glm::vec3(0.0f);
	bool first = true;


	// For each mesh section
	for (auto& m : meshSections)
	{
		// For each vertex in the current mesh section
		for (int i = 0; i < m.vertices.size(); i++)
		{
			if (first)
			{
				min = m.vertices[i].position;
				max = m.vertices[i].position;
				first = false;
			}
			else
			{
				glm::vec3 p = m.vertices[i].position;

				min.x = std::min(p.x, min.x);
				min.y = std::min(p.y, min.y);
				min.y = std::min(p.z, min.z);

				max.x = std::max(p.x, max.x);
				max.y = std::max(p.y, max.y);
				max.y = std::max(p.z, max.z);
			}
		}
	}

	minBounds = min;
	maxBounds = max;
}

void StaticMesh::CalculateMetaData()
{
	CalculateBounds();
}


StaticMesh::StaticMesh()
{
}


StaticMesh::~StaticMesh()
{
}


void StaticMesh::ProcessMeshSection(MeshSection* MeshSection, aiMesh * Mesh, const aiScene * Scene)
{
	// If null pointer then exit early
	if (MeshSection == nullptr)
	{
		Log::PrintError("Assimp: Null mesh section");
		return;
	}

	uint numVertices	= Mesh->mNumVertices;
	uint numFaces		= Mesh->mNumFaces;
	Log::PrintInfo("Processing mesh; " + std::to_string(numVertices) + " vertices; " + std::to_string(numFaces) + " faces");
	
	for (uint i = 0; i < numVertices; i++)
	{
		Vertex vertex;

		// Process vertex attributes
		vertex.position.x = Mesh->mVertices[i].x;
		vertex.position.y = Mesh->mVertices[i].y;
		vertex.position.z = Mesh->mVertices[i].z;
		vertex.normal.x = Mesh->mNormals[i].x;
		vertex.normal.y = Mesh->mNormals[i].y;
		vertex.normal.z = Mesh->mNormals[i].z;

		// If the mesh has tex coords
		if (Mesh->mTextureCoords[0])
		{
			vertex.texCoord.x = Mesh->mTextureCoords[0][i].x;
			vertex.texCoord.y = Mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.texCoord = glm::vec2(0.0f, 0.0f);
		}

		// If the mesh has vertex colours
		if (Mesh->mColors[0])
		{
			vertex.colour.x = Mesh->mColors[0][i].r;
			vertex.colour.y = Mesh->mColors[0][i].g;
			vertex.colour.z = Mesh->mColors[0][i].b;
			vertex.colour.w = Mesh->mColors[0][i].a;
		}

		// If the mesh has tangents
		if (Mesh->mTangents)
		{
			vertex.tangent.x = Mesh->mTangents[i].x;
			vertex.tangent.y = Mesh->mTangents[i].y;
			vertex.tangent.z = Mesh->mTangents[i].z;
		}

		MeshSection->vertices.push_back(vertex);
	}

	// Process indices
	for (uint i = 0; i < numFaces; i++)
	{
		aiFace face = Mesh->mFaces[i];
		for (uint j = 0; j < face.mNumIndices; j++)
		{
			MeshSection->indices.push_back(face.mIndices[j]);
		}
	}


	// Process material
	if (Mesh->mMaterialIndex >= 0)
	{

	}

	MeshSection->Construct(drawMode);
	Log::PrintInfo("Generated mesh section; " + std::to_string(MeshSection->vertices.size()) + " vertices; " + std::to_string(MeshSection->indices.size()) + " indices");
}


void StaticMesh::ProcessNode(aiNode* Node, const aiScene* Scene)
{
	Log::PrintInfo("Processing node '" + std::string(Node->mName.C_Str()) + "'; " + std::to_string(Node->mNumMeshes) + " meshes; " + std::to_string(Node->mNumChildren) + " children");

	// Process all the node's meshes (if there are any)
	for (uint i = 0; i < Node->mNumMeshes; i++)
	{
		aiMesh* sectionData = Scene->mMeshes[Node->mMeshes[i]];
		meshSections.push_back(MeshSection());
		ProcessMeshSection(&meshSections.back(), sectionData, Scene);
	}

	// Process each of its children (recursive)
	for (uint i = 0; i < Node->mNumChildren; i++)
	{
		ProcessNode(Node->mChildren[i], Scene);
	}
}


bool StaticMesh::Import(const std::string & Filename)
{
	ClearMeshSections();
	source = Filename;
	Log::PrintInfo("Attempting to load mesh from disk '" + Filename + "'..");

	if (GetDisplayName().size() <= 0 || GetDisplayName() == DEFAULT_DISPLAY_NAME)
	{
		auto path = std::filesystem::path(Filename);

		std::string name;
		if (path.has_stem())
		{
			name = path.stem().u8string();
		}
		else
		{
			name = path.filename().u8string();
		}

		SetDisplayName(name);
	}

	Assimp::Importer importer;

	// Build import flags
	uint importFlags = 0;
	for (auto it = importSettings.begin(); it != importSettings.end(); ++it)
	{
		auto& key = it->first;
		auto& value = it->second;
		if (value.value)
		{
			importFlags |= key;
		}
		else
		{
			//importFlags ^= key;
		}
	}
	
	const aiScene* scene = importer.ReadFile(Filename, importFlags);

	// Check scene root is not null, data is not incomplete
	//if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::stringstream ss;
		ss << "Assimp: " << importer.GetErrorString();
		Log::PrintError(ss.str(), 5.0f);
		return false;
	}

	ProcessNode(scene->mRootNode, scene);

	Log::PrintInfo("Loaded mesh from disk '" + Filename + "'\n"
		"		Mesh sections " + std::to_string(meshSections.size())
	);

	Construct();
	return true;
}

bool StaticMesh::Reimport()
{
	if (source.size() > 0)
	{
		auto materials = GetMaterials();
		bool success = Import(source);

		uint i = 0;
		for (auto& m : materials)
		{
			SetMaterial(i++, m);
		}

		if (success)
			Log::Print("Reimported StaticMesh '" + source + "'");

		return success;
	}
	else
	{
		return false;
	}
}


void StaticMesh::Draw(const glm::mat4& Transform)
{
	for (auto& m : meshSections)
	{
		if (m.material != nullptr)
		{
			m.material->Bind();
		}
		else
		{
			Shader::DefaultShader.Bind();
		}


		// Bind static mesh specific uniforms
		Shader* shaderProgram = Shader::GetCurrent();
		if (shaderProgram != nullptr && shaderProgram->IsValid())
		{
			// Set uniforms specific to this mesh
			shaderProgram->SetModelMatrix(Transform);

			// Move this to object?
			shaderProgram->SetVec3("MinBounds", minBounds);
			shaderProgram->SetVec3("MaxBounds", maxBounds);
		}

		m.Draw(drawMode);
	}
}

void StaticMesh::ClearMeshSections()
{
	for (auto& m : meshSections)
	{
		m.Destroy();
	}
	meshSections.clear();
}

void StaticMesh::Cleanup()
{
	// Clean-up references
	// ...

	ClearMeshSections();
}

void StaticMesh::Construct()
{
	CalculateMetaData();
}

std::vector<Material*> StaticMesh::GetMaterials() const
{
	std::vector<Material*> result;

	for (auto& m : meshSections)
	{
		result.push_back(m.material);
	}

	return result;
}

bool StaticMesh::SetMaterial(const uint & Index, Material* NewMaterial)
{
	if (Index < meshSections.size())
	{
		meshSections[Index].SetMaterial(NewMaterial);
		return true; // Return success
	}
	else
	{
		return false; // Return failure
	}
}

const std::vector<MeshSection>& StaticMesh::GetMeshSections() const
{
	return meshSections;
}

std::vector<MeshSection>& StaticMesh::GetMeshSections_Modifiable()
{
	return meshSections;
}

glm::vec3 StaticMesh::GetMinBounds() const
{
	return minBounds;
}

glm::vec3 StaticMesh::GetMaxBounds() const
{
	return maxBounds;
}

void StaticMesh::DrawDetails()
{
#if WITH_EDITOR
	if (ImGui::TreeNodeEx("Static Mesh", panelFlags))
	{
		ImGui::Selectable((GetDisplayName() + " (StaticMesh)").c_str());

		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			OpenWindow();
		}

		ImGui::TreePop();
	}
#endif
}

void StaticMesh::DrawWindow()
{
#if WITH_EDITOR
	const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;

	ImGui::Begin((GetDisplayName() + " (StaticMesh)").c_str(), &bIsVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

	// Menu bar
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();

			// Close window
			if (ImGui::MenuItem("Exit"))
			{
				SetVisible(false);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::Button("Reimport"))
		{
			Reimport();
		}

		ImGui::EndMenuBar();
	}

	// Materials
	
	if (ImGui::TreeNodeEx(("Materials (" + std::to_string(GetMaterials().size()) + ")").c_str(), panelFlags | ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto& meshSection : meshSections)
		{
			if (meshSection.material != nullptr)
			{
				meshSection.material->DrawDetails();
				AssetManager::DrawAssetBrowserContextMenu(&meshSection.material);
			}
			else
			{
				if (ImGui::TreeNodeEx("NULL", flags ^ ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
				{
					ImGui::TreePop();
				}
			}
		}

		ImGui::TreePop();
	}
	
	ImGui::Separator();
	ImGui::NewLine();
	ImGui::LabelText("Source", source.c_str());
	ImGui::SameLine();
	if (ImGui::Button("..."))
	{
		/*#if OS_Windows
					char filename[MAX_PATH] = "../";

					OPENFILENAME ofn;
					ZeroMemory(&filename, sizeof(filename));
					ZeroMemory(&ofn, sizeof(ofn));
					ofn.lStructSize = sizeof(ofn);
					ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
					ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
					ofn.lpstrFile = filename;
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrTitle = "Select a File, yo!";
					ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

					if (GetOpenFileNameA(&ofn))
					{
						std::cout << "You chose the file \"" << filename << "\"\n";
					}
		#endif*/
	}

	if (ImGui::TreeNodeEx("Import Settings", panelFlags ^ ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto it = importSettings.begin(); it != importSettings.end(); ++it)
		{
			auto& key = it->first;
			auto& value = it->second;
			ImGui::Checkbox(value.label.c_str(), &value.value);
		}


		ImGui::TreePop();
	}


	ImGui::NewLine();

	auto minBounds = GetMinBounds();
	auto maxBounds = GetMaxBounds();
	ImGui::Text(("Min Bounds (" + std::to_string(minBounds.x) + ", " + std::to_string(minBounds.y) + ", " + std::to_string(minBounds.z) + ")").c_str());
	ImGui::Text(("Max Bounds (" + std::to_string(maxBounds.x) + ", " + std::to_string(maxBounds.y) + ", " + std::to_string(maxBounds.z) + ")").c_str());


	ImGui::NewLine();


	// Mesh sections
	if (ImGui::TreeNodeEx(("Mesh Sections (" + std::to_string(GetMeshSections().size()) + ")").c_str(), panelFlags ^ ImGuiTreeNodeFlags_DefaultOpen))
	{
		uint i = 0;
		for (auto& m : GetMeshSections())
		{
			if (ImGui::TreeNodeEx(("Section [" + std::to_string(i) + "]").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
			{

				// IDs
				ImGui::Text(("VAO " + std::to_string(m.GetVAO())).c_str());
				ImGui::Text(("EBO " + std::to_string(m.GetEBO())).c_str());
				ImGui::Text(("VBO " + std::to_string(m.GetVBO())).c_str());

				// Vertices
				if (ImGui::TreeNodeEx(("Vertices (" + std::to_string(m.vertices.size()) + ")").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
				{
					uint j = 0;
					for (auto& v : m.vertices)
					{
						if (ImGui::TreeNodeEx(("[" + std::to_string(j) + "] (" + std::to_string(v.position.x) + ", " + std::to_string(v.position.y) + ", " + std::to_string(v.position.z) + ")").c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
						{

							ImGui::TreePop();
						}
						++j;
					}
					ImGui::TreePop();
				}

				// Indices
				if (ImGui::TreeNodeEx(("Indices (" + std::to_string(m.indices.size()) + ")").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
				{
					uint j = 0;
					for (auto& ind : m.indices)
					{
						if (ImGui::TreeNodeEx(("[" + std::to_string(j) + "] " + std::to_string(ind)).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
						{

							ImGui::TreePop();
						}
						++j;
					}
					ImGui::TreePop();
				}

				// Raw Vertices
				auto rawVertices = m.GetRawVertices();
				if (ImGui::TreeNodeEx(("Raw Vertices (" + std::to_string(rawVertices.size()) + ")").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
				{
					uint j = 0;
					for (auto& v : rawVertices)
					{
						if (ImGui::TreeNodeEx(("[" + std::to_string(j) + "] " + std::to_string(v)).c_str(), ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf))
						{

							ImGui::TreePop();
						}
						++j;
					}
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
			++i;
		}
		ImGui::TreePop();
	}

	ImGui::End();

#endif
}
