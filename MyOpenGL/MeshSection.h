#pragma once

#include "Transform.h"
#include "Object.h"
#include "Material.h"

#include <vector>
#include <set>
#include <algorithm>


/* Structure to contain vertex data */
class Vertex
{
public:
	// Properties
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec2 texCoord = glm::vec2(0.0f);
	glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 tangent = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::lowp_vec4 colour = glm::lowp_vec4(1.0f, 1.0f, 1.0f, 1.0f);


	Vertex() {};
	Vertex(const glm::vec3& Position)
		: position(Position) {};
	Vertex(const glm::vec3& Position, const glm::vec2& TexCoord)
		: position(Position), texCoord(TexCoord) {};
	Vertex(const glm::vec3& Position, const glm::vec2& TexCoord, const glm::vec3& Normal)
		: position(Position), texCoord(TexCoord), normal(Normal) {};
	~Vertex() {};

	bool operator==(const Vertex& A)
	{
		return (position == A.position && texCoord == A.texCoord && normal == A.normal && colour == A.colour);
	}
};


/* Mesh section. Mesh is comprised of list of mesh sections. */
class MeshSection// : public Object
{
private:
	GLuint VBO, VAO, EBO;


public:

	/* Return float stream of vertices */
	std::vector<GLfloat> GetRawVertices() const;

	// Properties
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
	Material* material; // We will change this to be stored in mesh sections


	enum DrawMode : uint
	{
		DrawElements = 0,
		DrawArrays = 1
	};


	// Constructors & destructors
	MeshSection();
	~MeshSection();


	// Methods
	void Construct(const DrawMode& DrawMode);
	void Draw(const DrawMode& DrawMode = DrawMode::DrawElements);
	void Destroy();
	void WeldAllVertices();
	void WeldVertices(const std::vector<int>& Indices);
	void RemoveIsolatedVertices();


	// Setters
	void AddVertex(const Vertex& NewVertex);
	void SetColour(const glm::lowp_vec4& Colour);


	// Getters
	GLuint GetVBO() const;
	GLuint GetVAO() const;
	GLuint GetEBO() const;
};

