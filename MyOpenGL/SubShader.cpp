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
		std::string path = std::filesystem::canonical(File).string(); // Get absolute (canonical) path of the shader file
		path = path.substr(0, path.find_last_of("\\"));				  // Remove filename to get absolute path

		// Isolate filename from directory
		std::string filename;
		std::size_t found = File.find_last_of("/");
		if (found != std::string::npos)
			filename = File.substr(found + 1);

		// Parse file
		std::string line;
		while (std::getline(inFile, line))
		{
			RemoveComments(line);

			if (line.find("#include") != std::string::npos) // Found an include
			{
				// Get file in-between " characters, i.e. #include "myfile"
				std::size_t first = line.find_first_of("\"");
				std::size_t last = line.find_last_of("\"");

				std::string subFilename = line.substr(first + 1, last - first - 1); // Remove quotes to isolate filename
				if (subFilename.size() > 0) // We found a substring
				{
					if (std::find(includes.begin(), includes.end(), subFilename) == includes.end()) // If we haven't already included this file
					{
						includes.push_back(subFilename); // Add to list of includes to ignore
						std::filesystem::path cachePath = std::filesystem::current_path(); // Cache the current working directory

						try { std::filesystem::current_path(path); }					   // This become our working directory
						catch (...) {}

						std::ifstream f;
						f.open(subFilename);											   // Try to open file
						std::filesystem::current_path(cachePath);						   // Restore working directory
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
					}
				}
			}
			else
				source.append(line + "\n");
		}


		if (OutputConsolidatedFile)
		{
			std::string outPath = path + "\\Compiled\\";
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
	std::ofstream inFile;
	inFile.open(Source); // Try to open file

	inFile.close();


	const GLchar* source_cstr = Source.c_str();
	ID = glCreateShader(shaderType);					// Create shader and return index
	glShaderSource(ID, 1, &source_cstr, NULL);			// Pass shader source
	glCompileShader(ID);								// Compile the shader

	GLchar infoLog[512];
	GLint success = GetLog(infoLog);

	if (success)
	{
		Log::Print("Sub-shader successfully compiled, TYPE:'" + GetShaderType() + "'");
	}
	else
	{
		Log::PrintError("Sub-shader failed to compile compiled, TYPE:'" + GetShaderType() + "'\n" + std::string(infoLog));
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
