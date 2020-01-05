#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "Utility.h"
#include "Object.h"

#include <sstream>
#include <filesystem>


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

