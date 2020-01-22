#include "Shader.h"


// ===================================== SUB-SHADER ============================================


void SubShader::RemoveComments(std::string& SourceLine)
{
	SourceLine = SourceLine.substr(0, SourceLine.find("//")); // Remove comments
	//SourceLine = SourceLine.substr(0, SourceLine.find("/*")); // Remove comments
}

void SubShader::RemoveCompilerDirective(std::string & SourceLine)
{
	SourceLine = SourceLine.substr(0, SourceLine.find("#version")); // Remove version directive
}


std::string SubShader::GetSource(const std::string& File, const bool& OutputConsolidatedFile)
{
	std::ifstream inFile;
	inFile.open(File);  // Try to open file
	std::string source; // String to dump to

	if (inFile.is_open()) // If open
	{
		std::vector<std::string> includes;							  // List of files we've already included


		// Isolate filename from directory
		std::string filename;
		std::string shaderFolder;
		std::size_t found = File.find_last_of("/");
		if (found != std::string::npos)
		{
			shaderFolder = File.substr(0, found);
			auto found2 = shaderFolder.find_last_of("/");
			if (found2 != std::string::npos)
				shaderFolder = shaderFolder.substr(found2 + 1);

			filename = File.substr(found + 1);
		}


		// Parse file
		std::string line;
		while (std::getline(inFile, line))
		{
			RemoveComments(line);

			if (line.find("#include") != std::string::npos) // Found an include
			{
				// Parse string
				std::size_t first = line.find_first_of("\"");
				std::size_t last = line.find_last_of("\"");

				std::string subFilename = line.substr(first + 1, last - first - 1); // Remove quotes to isolate filename
				if (subFilename.size() > 0) // We found a substring
				{
					if (std::find(includes.begin(), includes.end(), subFilename) == includes.end()) // If we haven't already included this file
					{
						includes.push_back(subFilename);									// Add to list of includes to ignore
						std::filesystem::path cachePath = std::filesystem::current_path();	// Cache the current working directory

						try { std::filesystem::current_path(std::filesystem::canonical(File).string()); } // This become our working directory
						catch (...) {}

						std::ifstream f;
						f.open(subFilename);												// Try to open file from local directory first


						// If we couldn't find the file from the local directory, try opening it from the SHADER_PATH in the engine directory
						if (!f.is_open())
						{
							std::filesystem::current_path(cachePath); // Restore working directory
							try { std::filesystem::current_path(SHADER_PATH); }
							catch (...) {}

							f.open(subFilename);
						}

						std::filesystem::current_path(cachePath);							// Restore working directory
						// If we managed to open the file, append the contents to our compiled shader file
						if (f.is_open())
						{
							std::string newLine;
							while (std::getline(f, newLine))
							{
								RemoveCompilerDirective(newLine);
								RemoveComments(newLine);
								source.append(newLine + "\n");
							}

							f.close();
						}
						else // Error
						{
							Log::PrintError("Failed to find shader include `" + subFilename + "`");
						}
					}
				}
			}
			else
			{
				source.append(line + "\n");
			}
		}


		if (OutputConsolidatedFile)
		{

			std::string outPath = SAVED_SHADERS_PATH + "/" + shaderFolder + "/";
			if (!std::filesystem::exists(outPath)) // If the directory doesn't already exist
				std::filesystem::create_directory(outPath);

			std::ofstream outFile;
			outFile.open(outPath + filename); // Open file to write to
			outFile << source;				  // Write consolidated source code to file
			outFile.close();
		}
	}

	inFile.close();
	return source;
}

SubShader::SubShader()
{
}

SubShader::SubShader(const GLenum& ShaderType)
	: shaderType(ShaderType)
{
	// Shaders source
	std::string source = GetSource("Vertex.glsl");
}


SubShader::~SubShader()
{
}

GLint SubShader::CompileSource(const std::string& Source)
{
	/*std::ofstream inFile;
	inFile.open(Source); // Try to open file

	inFile.close();*/


	const GLchar* source_cstr = Source.c_str();
	ID = glCreateShader(shaderType);					// Create shader and return index
	glShaderSource(ID, 1, &source_cstr, NULL);			// Pass shader source
	glCompileShader(ID);								// Compile the shader

	GLchar infoLog[512];
	GLint success = GetLog(infoLog);

	if (success)
	{
		Log::PrintInfo("'" + GetShaderType() + "' Sub-shader successfully compiled");
	}
	else
	{
		Log::PrintError("'" + GetShaderType() + "' Sub-shader failed to compile\n" + std::string(infoLog));
	}

	return success;
}


GLint SubShader::CompileFile(const std::string& File)
{
	std::string source = SubShader::GetSource(File);
	SetDisplayName(File.substr(File.rfind("/") + 1, File.length() - 1));
	return CompileSource(source);
}

GLint SubShader::GetLog(GLchar* InfoLog)
{
#define LOG_SIZE 512

	GLint success;
	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);

	if (!success)
		glGetShaderInfoLog(ID, LOG_SIZE, NULL, InfoLog);

	return success;
}

std::string SubShader::GetShaderType()
{
	if (shaderType == GL_FRAGMENT_SHADER)
		return "FRAGMENT";
	else
		return "VERTEX";
}





// ===================================== SHADER ============================================


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

		Log::PrintError("Failed to link sub-shaders '" + source + "'\n" + infoLog, 5.0f);
	}
	else
	{
		Log::PrintInfo("Successfully linked sub-shaders '" + source + "'");
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

	if (success)
		SetDisplayName(Folder.substr(Folder.rfind("/") + 1, Folder.length() - 1));

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
			Log::PrintInfo("Recompiling '" + (*it)->source + "'");
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
