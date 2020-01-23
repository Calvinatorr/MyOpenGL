#include "StaticMesh.h"



std::vector<std::string> StaticMesh::ConvertToTokens(const std::string & Line, const char & Deliminator)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(Line);

	while (std::getline(tokenStream, token, Deliminator))
		tokens.push_back(token);

	return tokens;
}

int StaticMesh::FindToken(const std::vector<std::string>& Tokens, const std::string & Snippet)
{
	for (unsigned int i = 0; i < Tokens.size(); i++)
		if (Tokens[i].find(Snippet) != std::string::npos) // Found
			return i;

	return -1; // Fail
}

std::string StaticMesh::CleanupToken(std::string Token)
{
	const char offendingCharacters[] = "[,";
	for (char c : offendingCharacters)
		Token.erase(std::remove(Token.begin(), Token.end(), c), Token.end());

	return Token;
}

float StaticMesh::FloatToken(std::string Token)
{
	Token = CleanupToken(Token);
	return std::stof(Token);
}

int StaticMesh::IntToken(std::string Token)
{
	Token = CleanupToken(Token);
	return std::stoi(Token);
}

glm::vec3 StaticMesh::Vec3Token(const std::vector<std::string>& Tokens)
{
	return glm::vec3(FloatToken(Tokens[1]), FloatToken(Tokens[2]), FloatToken(Tokens[3]));
}

glm::vec2 StaticMesh::Vec2Token(const std::vector<std::string>& Tokens)
{
	return glm::vec2(FloatToken(Tokens[1]), FloatToken(Tokens[2]));
}

std::vector<int> StaticMesh::DecodeIndicesFromToken(const std::string & Token)
{
	std::vector<std::string> pt = ConvertToTokens(Token, '/');
	std::vector<int> indices;

	if (pt.size() > 0)
	{
		for (unsigned int i = 0; i < pt.size(); i++)
		{
			try // We try to convert the current token to an int and push it back. If we fail then ignore it.
			{
				indices.push_back(std::stoi(pt[i]));
			}
			catch (...)
			{

			}
		}
	}

	return indices;
}


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


void StaticMesh::LoadMeshObj(const std::string & File)
{
	/*std::ifstream inFile;
	inFile.open(File); // Try to open file
	source = File;	   // Reference to source file in-case we ever need to reload this

	if (inFile.is_open()) // If open
	{
		vertices.clear();
		std::string line;


		// Cached attributes
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;

		auto VertexFromIndex = [&](const std::string& Token)
		{
			std::vector<int> newIndices = DecodeIndicesFromToken(Token); // Try to decode

			if (newIndices.size() > 0) // If we successfully decoded the token, then we add a vertex and point to it
			{
				int index = 0;

				// Iteratively add attributes. We can miss out attributes from the file by doing this which is safer.
				Vertex v;
				if (positions.size() > 0)
					v.position = positions[newIndices[index++] - 1];
				if (texCoords.size() > 0)
					v.texCoord = texCoords[newIndices[index++] - 1];
				if (normals.size() > 0)
					v.normal = normals[newIndices[index++] - 1];

				// Look to find if the vertex already exists
				int found = -1;
				for (unsigned int i = 0; i < vertices.size(); i++)
				{
					if (v == vertices[i])
					{
						found = i; // Doubles as our index
						break;	   // Premature break, we got we needed
					}
				}

				if (0 <= found) // If we found a vertex
				{
					indices.push_back(found);
				}
				else // Otherwise we add a new vertex and push back an index to the end
				{
					vertices.push_back(v);
					indices.push_back(vertices.size() - 1);
				}
			}
		};


		// Parse file & first construct geometry
		while (std::getline(inFile, line))
		{
			std::vector<std::string> tokens = ConvertToTokens(line, ' '); // Parse line into tokens by splitting whitespaces


			// Parse line
			if (tokens[0] == "v") // Position
				positions.push_back(Vec3Token(tokens));
			else if (tokens[0] == "vt") // TexcCoord
				texCoords.push_back(Vec2Token(tokens));
			else if (tokens[0] == "vn") // Normal
				normals.push_back(Vec3Token(tokens));
			else if (tokens[0] == "#")
			{
				if (FindToken(tokens, "Bounds") > -1) // Parse render bounds
				{
					minBounds = glm::vec3(FloatToken(tokens[2]), FloatToken(tokens[3]), FloatToken(tokens[4]));
					maxBounds = glm::vec3(FloatToken(tokens[6]), FloatToken(tokens[7]), FloatToken(tokens[8]));
				}
			}
			else if (tokens[0] == "f") // End of geometry, start pairing up vertices
			{
				VertexFromIndex(tokens[1]);
				VertexFromIndex(tokens[2]);
				VertexFromIndex(tokens[3]);
			}
		}


		Log::PrintInfo("StaticMesh successfully loaded '" + File + "\n" +
			"	" + std::to_string(vertices.size()) + " vertices\n" +
			"	" + std::to_string(indices.size() / 3) + " faces");
	}
	else
	{
		Log::PrintError("StaticMesh failed to load '" + File + "'");
	}

	inFile.close();

	// Optimise StaticMesh
	//WeldAllVertices();
	//RemoveIsolatedVertices();

	Construct();*/
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


bool StaticMesh::LoadMeshFromDisk(const std::string & Filename)
{
	Clear();
	source = Filename;
	Log::PrintInfo("Attempting to load mesh from disk '" + Filename + "'..");

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(Filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	// Check scene root is not null, data is not incomplete
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::stringstream ss;
		ss << "Assimp: " << importer.GetErrorString() << std::endl;
		Log::PrintError(ss.str());
		return false;
	}

	std::string directory = Filename.substr(0, Filename.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);

	Log::PrintInfo("Loaded mesh from disk '" + Filename + "'"
	"\n		Mesh sections " + std::to_string(meshSections.size())
	);

	CalculateMetaData();

	return true;
}

bool StaticMesh::Reimport()
{
	if (source.size() > 0)
	{
		auto materials = GetMaterials();
		bool success = LoadMeshFromDisk(source);

		uint i = 0;
		for (auto& m : materials)
		{
			SetMaterial(i++, m);
		}

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

void StaticMesh::Clear()
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

	Clear();
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
		meshSections[Index].material = NewMaterial;
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

glm::vec3 StaticMesh::GetMinBounds() const
{
	return minBounds;
}

glm::vec3 StaticMesh::GetMaxBounds() const
{
	return maxBounds;
}

void StaticMesh::DrawGUI()
{
	if (ImGui::TreeNodeEx("Static Mesh", panelFlags))
	{
		ImGui::LabelText("Static Mesh Asset", GetDisplayName().c_str());

		ImGui::NewLine();

		ImGui::LabelText("Source", source.c_str());
		if (ImGui::Button("Reimport from Disk"))
		{
			Reimport();
		}
		ImGui::SameLine();
		if (ImGui::Button("Import from Source"))
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

		ImGui::NewLine();

		auto minBounds = GetMinBounds();
		auto maxBounds = GetMaxBounds();
		ImGui::Text(("Min Bounds (" + std::to_string(minBounds.x) + ", " + std::to_string(minBounds.y) + ", " + std::to_string(minBounds.z) + ")").c_str());
		ImGui::Text(("Max Bounds (" + std::to_string(maxBounds.x) + ", " + std::to_string(maxBounds.y) + ", " + std::to_string(maxBounds.z) + ")").c_str());


		ImGui::NewLine();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
		if (ImGui::TreeNodeEx(("Materials (" + std::to_string(GetMaterials().size()) + ")").c_str(), flags))
		{
			// We will change this when multiple materials are functional
			ImGui::Text("0");
			ImGui::SameLine();

			for (auto& m : GetMaterials())
			{
				std::string materialName = "NULL_MATERIAL";
				std::string shaderName = "NULL_SHADER";
				if (m != nullptr)
				{
					materialName = m->GetDisplayName();
					if (m->shader != nullptr)
						shaderName = m->shader->GetDisplayName();
				}

				if (ImGui::TreeNodeEx((materialName + " (" + shaderName + ")").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
				{
					m->DrawGUI();
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx(("Mesh Sections (" + std::to_string(GetMeshSections().size()) + ")").c_str(), flags ^ ImGuiTreeNodeFlags_DefaultOpen))
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

		ImGui::TreePop();
	}
}
