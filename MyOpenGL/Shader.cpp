#include "Shader.h"

Shader* Shader::current = nullptr;
std::set<Shader*> Shader::all = std::set<Shader*>();

void Shader::Cleanup()
{
	auto it = all.begin();
	while (it != all.end())
	{
		if ((*it) != nullptr)
			(*it)->Destroy();

		it++; // Move to next element in set
	}
}

void Shader::Create()
{
	//all.push_back(this);
	all.insert(this);
}

Shader::Shader()
{

}

Shader::Shader(const std::string & Folder)
{
	Create();
	CompileShadersFromFolder(Folder);
}


Shader::~Shader()
{
	Destroy();
}

GLint Shader::CompileShadersFromFolder(const std::string & Folder)
{
	GLint success = 1;
	
	// Shaders
	vertexShader = SubShader(GL_VERTEX_SHADER);
	success = vertexShader.CompileFile(Folder + "/Vertex.glsl") && success;
	fragmentShader = SubShader(GL_FRAGMENT_SHADER);
	success = fragmentShader.CompileFile(Folder + "/Fragment.glsl") && success;

	source = Folder;
	Create();

	return success;
}

GLint Shader::LinkShaders()
{
	// Shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader.GetID());
	glAttachShader(ID, fragmentShader.GetID());
	glLinkProgram(ID); // Link our shaders to the shader program

	// Shader program debugging
	GLint success;
	GLchar infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);

		Log::PrintError("Failed to link sub-shaders '" + source + "'\n" + infoLog);
	}
	else
	{
		Log::Print("Successfully linked sub-shaders '" + source + "'");
	}

	glDeleteShader(vertexShader.GetID());
	glDeleteShader(fragmentShader.GetID());

	return success;
}

GLint Shader::Compile(const std::string & Folder)
{
	GLint success = 1;
	success = success && CompileShadersFromFolder(Folder);
	LinkShaders();

	return success;
}

GLint Shader::Recompile()
{
	GLint success = 1;
	if (!source.empty())
		success = success && Compile(source);

	return success;
}

void Shader::RecompileAll()
{
	std::cout << std::to_string(all.size()) << std::endl;

	auto it = all.begin();
	while (it != all.end())
	{
		if (*it != nullptr)
		{
			(*it)->Recompile();
			Log::Print("Recompiling '" + (*it)->source + "'");
		}

		it++; // Move to next element in set
	}
}

void Shader::Bind()
{
	if (current != nullptr)
		Unbind();


	glUseProgram(ID);
	current = this;

	Camera* camera = Camera::GetCurrent();
	if (camera != nullptr)
		camera->Draw(); // Bind the view matrices
}

void Shader::Destroy()
{
	Unbind();
	/*std::vector<Shader*>::iterator pos = std::find(Shader::all.begin(), Shader::all.end(), this);
	if (pos != Shader::all.end())
		Shader::all.erase(pos);*/
}

void Shader::SetBool(const GLchar* Name, const GLboolean& Value) const
{
	SetInt(Name, Value);
}

void Shader::SetInt(const GLchar* Name, const GLint& Value) const
{
	glUniform1i(GetUniformLocation(Name), Value);
}

void Shader::SetFloat(const GLchar * Name, const GLfloat& Value) const
{
	glUniform1f(GetUniformLocation(Name), Value);
}

void Shader::SetVec3(const GLchar * Name, const glm::vec3 & Value) const
{
	glUniform3f(GetUniformLocation(Name), Value.x, Value.y, Value.z);
}

void Shader::SetVec4(const GLchar * Name, const glm::vec4 & Value) const
{
	glUniform4f(GetUniformLocation(Name), Value.x, Value.y, Value.z, Value.w);
}

void Shader::SetMatrix4x4(const GLchar * Name, const glm::mat4& Value) const
{
	glUniformMatrix4fv(GetUniformLocation(Name), 1, GL_FALSE, glm::value_ptr(Value));
}

void Shader::SetModelMatrix(const glm::mat4 & Model) const
{
	SetMatrix4x4(MODEL_MATRIX, Model);
}

void Shader::SetViewMatrix(const glm::mat4& View) const
{
	SetMatrix4x4(VIEW_MATRIX, View);
}

void Shader::SetProjectionMatrix(const glm::mat4& Projection) const
{
	SetMatrix4x4(PROJECTION_MATRIX, Projection);
}

void Shader::SetCameraPosition(const glm::vec3 & ViewPosition) const
{
	SetVec3("CameraPosition", ViewPosition);
}

void Shader::SetCameraDirection(const glm::vec3 & ViewDirection) const
{
	SetVec3("CameraDirection", ViewDirection);
}

GLint Shader::GetUniformLocation(const GLchar* Name) const
{
	if (IsValid())
		return glGetUniformLocation(ID, Name);
	else
		return -1;
}

Shader * Shader::GetCurrent()
{
	return current;
}

void Shader::Unbind()
{
	if (current != nullptr)
		current = nullptr; // Clear pointer

	glUseProgram(-1); // Unbind from OpenGL
}
