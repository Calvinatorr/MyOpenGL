#include "Mesh.h"



std::vector<std::string> Mesh::ConvertToTokens(const std::string & Line, const char & Deliminator)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(Line);

	while (std::getline(tokenStream, token, Deliminator))
		tokens.push_back(token);

	return tokens;
}

int Mesh::FindToken(const std::vector<std::string>& Tokens, const std::string & Snippet)
{
	for (unsigned int i = 0; i < Tokens.size(); i++)
		if (Tokens[i].find(Snippet) != std::string::npos) // Found
			return i;

	return -1; // Fail
}

std::string Mesh::CleanupToken(std::string Token)
{
	const char offendingCharacters[] = "[,";
	for (char c : offendingCharacters)
		Token.erase(std::remove(Token.begin(), Token.end(), c), Token.end());

	return Token;
}

float Mesh::FloatToken(std::string Token)
{
	Token = CleanupToken(Token);
	return std::stof(Token);
}

int Mesh::IntToken(std::string Token)
{
	Token = CleanupToken(Token);
	return std::stoi(Token);
}

glm::vec3 Mesh::Vec3Token(const std::vector<std::string>& Tokens)
{
	return glm::vec3(FloatToken(Tokens[1]), FloatToken(Tokens[2]), FloatToken(Tokens[3]));
}

glm::vec2 Mesh::Vec2Token(const std::vector<std::string>& Tokens)
{
	return glm::vec2(FloatToken(Tokens[1]), FloatToken(Tokens[2]));
}

std::vector<int> Mesh::DecodeIndicesFromToken(const std::string & Token)
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

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}


void Mesh::LoadMeshObj(const std::string & File)
{
	std::ifstream inFile;
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

			// Print file tokens
			/*for (int i = 0; i < tokens.size(); i++)
				std::cout << tokens[i] << "||";
			std::cout << std::endl;*/


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
				/*
				if (tokens.size() > 4) // Quad, size includes f token
				{
					VertexFromIndex(tokens[4]);
				}
				else
				{

				}*/
			}
		}


		/*// Parse file again after we've construced geometry. This is safer to avoid invalid accessing of memory.
		inFile.clear(); // Clear stream
		inFile.seekg(0, std::ios::beg); // Reset
		while (std::getline(inFile, line))
		{
			std::vector<std::string> tokens = ConvertToTokens(line, ' '); // Parse line into tokens by splitting whitespaces

			// Parse line
			if (tokens[0] == "f") // End of geometry, start pairing up vertices
			{
				VertexFromIndex(tokens[1]);
				VertexFromIndex(tokens[2]);
				VertexFromIndex(tokens[3]);

				//if (tokens.size() > 4) // Quad, size includes f token
				//	VertexFromIndex(tokens[4]);
			}
		}*/

		Log::Print("Mesh successfully loaded '" + File + "\n" +
			"	" + std::to_string(vertices.size()) + " vertices\n" +
			"	" + std::to_string(indices.size() / 3) + " faces");
	}
	else
	{
		Log::PrintError("Mesh failed to load '" + File + "'");
	}

	inFile.close();

	// Optimise mesh
	//WeldAllVertices();
	//RemoveIsolatedVertices();

	Construct();
}