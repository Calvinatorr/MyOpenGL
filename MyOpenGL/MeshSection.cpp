#include "MeshSection.h"

std::set<MeshSection*> MeshSection::all = std::set<MeshSection*>();


std::vector<GLfloat> MeshSection::GetRawVertices() const
{
	std::vector<GLfloat> rawVertices;

	// Lambdas
	auto AddVec3 = [&] (glm::vec3 v)
	{
		rawVertices.push_back(v.x);
		rawVertices.push_back(v.y);
		rawVertices.push_back(v.z);
	};

	auto AddVec2 = [&](glm::vec2 v)
	{
		rawVertices.push_back(v.x);
		rawVertices.push_back(v.y);
	};

	
	// Convert attributes to vertex list
	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex v = vertices[i];

		AddVec3(v.position);
		AddVec2(v.texCoord);
		AddVec3(v.colour);
		AddVec3(v.normal);
	}

	return rawVertices;
}

MeshSection::MeshSection()
{
	
}


MeshSection::~MeshSection()
{
	Destroy();
}

void MeshSection::Construct()
{
	Destroy(); // First destroy to make sure we don't cause a memory leak

	// Convert vectors to array data
	std::vector<GLfloat> rawVertices = GetRawVertices(); // Get raw vertex data as list of floats
	GLfloat* rawVerticesPt = rawVertices.data();		 // Convert list of floats to array (pointer to memory). We don't know the size of this without the vector.
	GLuint* indicesPt = indices.data();					 // Convert list of uints to array


	// Generate vertex attribute array object & buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO); // Buffer for vertices
	glBindVertexArray(VAO);

	// EBO (indices)
	if (drawMode == DrawMode::DrawElements)
	{
		glGenBuffers(1, &EBO); // Buffer for indices (element buffer)

		// Bind indices buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indicesPt, GL_STATIC_DRAW);
	}


	// Bind vertices buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * rawVertices.size(), rawVerticesPt, GL_STATIC_DRAW);


	// Attributes
	GLuint attrib = 0;
	GLuint memorySize = 11 * sizeof(GLfloat);
	GLuint stride = 0;

	auto AddAttribute = [&](GLuint Size, GLenum Normalized = GL_FALSE)
	{
		glVertexAttribPointer(attrib, Size, GL_FLOAT, Normalized, memorySize, (void*)stride); // change me
		stride += Size * sizeof(GLfloat);
		glEnableVertexAttribArray(attrib);
		attrib++;
	};

	AddAttribute(3); // Position
	AddAttribute(2); // TexCoord
	AddAttribute(3); // Colour
	AddAttribute(4, GL_TRUE); // Normal

	CalculateBounds();

	all.insert(this); // Insert unique reference to this MeshSection, set so each element is unique
}

void MeshSection::Draw(const glm::mat4& Transform)
{
	// Draw mesh section
	if (material != nullptr) // If we have a material assigned
		material->Bind();	 // Then bind that material, will bind the necessary shader


	Shader* shaderProgram = Shader::GetCurrent();
	if (shaderProgram != nullptr && shaderProgram->IsValid())
	{
		// Set uniforms specific to this mesh
		shaderProgram->SetModelMatrix(Transform);

		// Move this to object?
		shaderProgram->SetVec3("MinBounds", minBounds);
		shaderProgram->SetVec3("MaxBounds", maxBounds);
	}


	// render container
	glBindVertexArray(VAO);
	
	// Allow for drawing without EBO
	switch (drawMode)
	{
	case MeshSection::DrawMode::DrawElements:
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0); // Consolidate me for variable size
		break;
	default:
	case MeshSection::DrawMode::DrawArrays:
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
		break;
	}
}

/*void MeshSection::Draw()
{
	Draw(transform.GetMatrix());
}*/

void MeshSection::Destroy()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void MeshSection::WeldAllVertices()
{
	/*std::cout << "Pre-optimised mesh stats:" << std::endl;
	std::cout << "vertices size = " << std::to_string(vertices.size()) << std::endl;
	std::cout << "indices size = " << std::to_string(indices.size()) << std::endl;
	std::cout << "indices:" << std::endl;
	for (int i = 0; i < indices.size(); i++)
		std::cout << std::to_string(indices[i]) << "||";
	std::cout << std::endl;

	std::vector<Vertex> cached(vertices); // Copy vector so we can be safe when iterating vertices
	for (int i = cached.size() - 1; i > 0; i--)
	{
		// weld
	}


	std::cout << "Optimised mesh stats:" << std::endl;
	std::cout << "vertices size = " << std::to_string(vertices.size()) << std::endl;
	std::cout << "indices size = " << std::to_string(indices.size()) << std::endl;
	std::cout << "indices:" << std::endl;
	for (int i = 0; i < indices.size(); i++)
		std::cout << std::to_string(indices[i]) << "||";
	std::cout << std::endl;*/
}

void MeshSection::WeldVertices(const std::vector<int>& Indices)
{

}

void MeshSection::RemoveIsolatedVertices()
{
	if (vertices.size() > 0)
	{
		for (int i = vertices.size() - 1; i >= 0; i--)				   // For each vertex
		{
			if (std::find(indices.begin(), indices.end(), i) >= indices.end()) // If no indices point to this vertex
				vertices.erase(vertices.begin() + i);
		}
	}
}

void MeshSection::CalculateBounds()
{
	glm::vec3 min = glm::vec3(0.0f), max = glm::vec3(0.0f);
	GLboolean first = GL_TRUE;

	for (int i = 0; i < vertices.size(); i++)
	{
		if (first)
		{
			min = vertices[i].position;
			max = vertices[i].position;
			first = GL_FALSE;
		}
		else
		{
			glm::vec3 p = vertices[i].position;

			min.x = std::min(p.x, min.x);
			min.y = std::min(p.y, min.y);
			min.y = std::min(p.z, min.z);

			max.x = std::max(p.x, max.x);
			max.y = std::max(p.y, max.y);
			max.y = std::max(p.z, max.z);
		}
	}

	minBounds = min;
	maxBounds = max;
}

void MeshSection::Cleanup()
{
	auto it = all.begin();
	while (it != all.end())
	{
		if ((*it) != nullptr)
			(*it)->Destroy();

		it++; // Move to next element in set
	}
}

void MeshSection::AddVertex(const Vertex & NewVertex)
{
	vertices.push_back(NewVertex);
}

void MeshSection::SetColour(const glm::vec3 & Colour)
{
	for (int i = 0; i < vertices.size(); i++)
		vertices[i].colour = Colour;
}

GLuint MeshSection::GetVBO() const
{
	return VBO;
}

GLuint MeshSection::GetVAO() const
{
	return VAO;
}

GLuint MeshSection::GetEBO() const
{
	return EBO;
}

glm::vec3 MeshSection::GetMinBounds() const
{
	return minBounds;
}

glm::vec3 MeshSection::GetMaxBounds() const
{
	return maxBounds;
}
