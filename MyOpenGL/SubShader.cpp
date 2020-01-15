#include "SubShader.h"



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
		Log::Print("shaderFolder `" + shaderFolder + "`");
		Log::Print("filename `" + filename + "`");


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
							catch (...) { }

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
		Log::Print("'" + GetShaderType() + "' Sub-shader successfully compiled");
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
