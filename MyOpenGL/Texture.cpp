#include "Texture.h"

Texture* Texture::current = nullptr;

void Texture::Generate() // Add to our texture pool
{
	GLuint cachedID;
	glGenTextures(1, &cachedID);
	ID = cachedID;

	SetType(type);

	Log::PrintInfo("Texture object generated, ID:'" + std::to_string(ID) + "'");
}

void Texture::Init()
{
	Generate(); // Generate the texture so we can bind the ID
	Bind(); // First bind so we can set the properties

	SetWrapMode(wrapMode);
	SetFilter(filter);
}


Texture::Texture()
{
	Init();
}

Texture::Texture(const GLchar * File)
{
	Init();
	LoadResource(File);
}


Texture::~Texture()
{
}

void Texture::Bind()
{
	if (IsValid())
	{
		if (current != this)
		{
			//glActiveTexture(GL_TEXTURE0);
			glBindTexture(type, ID);
			current = this;
		}
	}
	else
	{
		Log::PrintError("Texture object failed to find '" + source + "'");
	}
}

void Texture::Unbind()
{
	/*Bind();
	glDisable(type);*/

	ID = -1;
}

void Texture::SetType(const Texture::Type & TextureType)
{
	type = TextureType;
}

void Texture::SetWrapMode(const Texture::WrapMode & TextureWrapMode)
{
	Bind();
	wrapMode = TextureWrapMode;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
}

void Texture::SetFilter(const Texture::Filter & TextureFilter)
{
	Bind();
	filter = TextureFilter;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
}


GLint Texture::LoadResource(const GLchar* File)
{
	GLint success = -1; // Default to fail
	source = File;

	if (IsValid()) // If we have valid texture memory allocated first
	{
		stbi_set_flip_vertically_on_load(FlipVertical); // Fix flipping
		unsigned char* data = stbi_load(File, &width, &height, &numOfChannels, 0);

		if (data) // If the data is valid
		{
			glTexImage2D(type, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			if (GenerateMipMaps)
				glGenerateMipmap(type);

			Log::PrintInfo("Texture successfully loaded '" + std::string(File) + "'");
			success = 1;
		}
		else
		{
			Log::PrintError("Failed to load texture '" + std::string(File) + "'");
		}

		stbi_image_free(data); // Free up memory


		if (displayName.length() <= 0 || displayName == DEFAULT_DISPLAY_NAME)
		{
			std::string name = File;
			name = name.substr(name.rfind("/") + 1, name.length() - 1);
			name = name.substr(0, name.find("."));
			SetDisplayName(name);
		}
	}
	else
	{
		Log::PrintError("Texture object invalid '" + std::string(File) + "'");
	}

	return success;
}

void Texture::SetFormat(const Format & NewFormat, const GLboolean & SetInternalFormat)
{
	format = NewFormat;
	if (SetInternalFormat)
		internalFormat = NewFormat;
}

void Texture::SetFormatHDR()
{
	format = Format::RGB;
	internalFormat = Format::HDR;
}

GLboolean Texture::IsValid() const
{
	return ID >= 1;
}

Texture::Type Texture::GetType() const
{
	return type;
}

Texture::WrapMode Texture::GetWrapMode() const
{
	return wrapMode;
}

Texture::Filter Texture::GetFilter() const
{
	return filter;
}

GLint Texture::GetWidth() const
{
	return width;
}

GLint Texture::GetHeight() const
{
	return height;
}

GLint Texture::GetNumberOfChannels() const
{
	return numOfChannels;
}