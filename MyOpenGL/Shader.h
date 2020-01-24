#pragma once

// GLFW
#include <glad/glad.h> 
#include <GLFW/glfw3.h>


// GLM Mathematics library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm/gtx/string_cast.hpp"


#include "Editor.h"
#include "Utility.h"
#include "Asset.h"
#include "Texture.h"
#include "Camera.h"


#include <set>
#include <algorithm>
#include <sstream>
#include <filesystem>


// ===================================== SUB-SHADER ============================================

#ifndef SHADER_PATH
#define SHADER_PATH std::string(ENGINE_PATH + "Shaders/")
#endif

#ifndef SAVED_SHADERS_PATH
#define SAVED_SHADERS_PATH std::string(SAVED_PATH + "Shaders/")
#endif


class SubShader : public Object
{
private:
	static void RemoveComments(std::string& SourceLine);
	static void RemoveCompilerDirective(std::string& SourceLine);


protected:
	GLenum shaderType = GL_FRAGMENT_SHADER;


public:
	static std::string GetSource(const std::string& File, const bool& OutputConsolidatedFile = true);


	SubShader();
	SubShader(const GLenum& ShaderType);
	~SubShader();


	// Methods
	GLint CompileSource(const std::string& Source);
	GLint CompileFile(const std::string& File);


	// Debugging
	GLint GetLog(GLchar* InfoLog);
	std::string GetShaderType();
};



// ===================================== SHADER ============================================

class Shader : public Asset, public _BindableBase
{
private:
	static Shader* current;
	static std::set<Shader*> all;
	void Create();

public:

	// Compiler constants
	const GLchar* MODEL_MATRIX = "Model";
	const GLchar* VIEW_MATRIX = "View";
	const GLchar* PROJECTION_MATRIX = "Projection";


	// Properties
	SubShader vertexShader, fragmentShader; // All sub-shaders
	std::string source;


	// Constructors & destructors
	Shader();
	Shader(const std::string& Folder);
	~Shader();
	

	// Methods
	GLint CompileShadersFromFolder(const std::string& Folder);
	GLint LinkShaders();
	GLint Compile(const std::string& Folder);
	GLint Recompile();
	static void RecompileAll();
	void Bind() override;
	void Destroy();
	static void Cleanup();


	// Setters
	void SetBool(const GLchar* Name, const GLboolean& Value) const;
	void SetInt(const GLchar* Name, const GLint& Value) const;
	void SetFloat(const GLchar* Name, const GLfloat& Value) const;
	void SetVec3(const GLchar* Name, const glm::vec3& Value) const;
	void SetVec4(const GLchar* Name, const glm::vec4& Value) const;
	void SetMatrix4x4(const GLchar* Name, const glm::mat4& Value) const;
	//void SetTextureSampler(const GLchar* Name, const Texture& Tex) const;


	// Matrices
	void SetModelMatrix(const glm::mat4& Model) const;
	void SetViewMatrix(const glm::mat4& View) const;
	void SetProjectionMatrix(const glm::mat4& Projection) const;
	void SetCameraPosition(const glm::vec3& ViewPosition) const;
	void SetCameraDirection(const glm::vec3& ViewDirection) const;


	// Getters
	GLint GetUniformLocation(const GLchar* Name) const;
	static Shader* GetCurrent();
	static void UnbindCurrent(); // Call at the end of the render thread
};