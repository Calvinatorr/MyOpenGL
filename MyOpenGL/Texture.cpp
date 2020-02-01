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

Texture::Texture(const std::string& Filename)
{
	Init();
	Import(Filename);
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


bool Texture::Import(const std::string& Filename)
{
	bool bSuccess = false; // Default to fail
	source = Filename;

	if (IsValid()) // If we have valid texture memory allocated first
	{
		stbi_set_flip_vertically_on_load(bFlipVertical); // Fix flipping
		unsigned char* data = stbi_load(Filename.c_str(), &width, &height, &numOfChannels, 0);

		if (data) // If the data is valid
		{
			glTexImage2D(type, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			if (bGenerateMipMaps)
				glGenerateMipmap(type);

			Log::PrintInfo("Texture successfully loaded '" + Filename + "'");
			bSuccess = true;
		}
		else
		{
			Log::PrintError("Failed to load texture '" + Filename + "'");
		}

		stbi_image_free(data); // Free up memory


		if (displayName.length() <= 0 || displayName == DEFAULT_DISPLAY_NAME)
		{
			std::string name = Filename;
			name = name.substr(name.rfind("/") + 1, name.length() - 1);
			name = name.substr(0, name.find("."));
			SetDisplayName(name);
		}
	}
	else
	{
		Log::PrintError("Texture object invalid '" + Filename + "'");
	}

	return bSuccess;
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

glm::vec2 Texture::GetSize() const
{
	return glm::vec2(width, height);
}

GLint Texture::GetNumberOfChannels() const
{
	return numOfChannels;
}

void Texture::DrawWindow()
{
	BeginAssetWindow();


	double ratio = width / height;
	ImGui::Image(ImTextureID(GetID()), ImVec2(512, 512 / ratio), ImVec2(0, 1), ImVec2(1, 0)); // Flip UV

	ImGui::NewLine();
	ImGui::Separator();
	ImGui::NewLine();

	if (Editor::DrawPanel("Settings"))
	{
		ImGui::Checkbox("Generate Mipmaps", &bGenerateMipMaps);
		ImGui::Checkbox("Flip Vertical", &bFlipVertical);

		// Add wrap mode, format, filter etc here when data can be reflected in C++
	}

	EndAssetWindow();
}
