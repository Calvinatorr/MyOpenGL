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
	glm::vec2 texCoord = glm::vec3(0.0f);
	glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);


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
class MeshSection : public Object
{
private:
	static std::set<MeshSection*> all;
	GLuint VBO, VAO, EBO;

protected:
	std::vector<GLfloat> GetRawVertices() const;
	glm::vec3 minBounds, maxBounds;

public:
	// Properties
	//Transform transform;
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	Material* material; // We will change this to be stored in mesh sections
	enum DrawMode : GLuint
	{
		DrawElements = 0,
		DrawArrays = 1
	} drawMode = DrawMode::DrawElements;


	// Constructors & destructors
	MeshSection();
	~MeshSection();


	// Methods
	void Construct();
	void Draw(const glm::mat4& Transform);
	//void Draw();
	void Destroy();
	void WeldAllVertices();
	void WeldVertices(const std::vector<int>& Indices);
	void RemoveIsolatedVertices();
	void CalculateBounds(); // IMPLMENT ME
	static void Cleanup();


	// Setters
	void AddVertex(const Vertex& NewVertex);
	void SetColour(const glm::vec3& Colour);


	// Getters
	GLuint GetVBO() const;
	GLuint GetVAO() const;
	GLuint GetEBO() const;
	glm::vec3 GetMinBounds() const;
	glm::vec3 GetMaxBounds() const;
};

