#pragma once

#include "Utility.h"
#include "Asset.h"

#include "stb_image.h"


class Texture : public Asset, public _BindableBase
{

public:
	enum Type : GLenum // Wrapper for texture types
	{
		Texture1D = GL_TEXTURE_1D,
		Texture1DArray = GL_TEXTURE_1D_ARRAY,
		Texture2D = GL_TEXTURE_2D,
		Texture2DArray = GL_TEXTURE_2D_ARRAY,
		Texture2DMultiSample = GL_TEXTURE_2D_MULTISAMPLE,
		Texture2DMultiSampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
		Texture3D = GL_TEXTURE_3D,
		TextureCube = GL_TEXTURE_CUBE_MAP,
		TextureCubeArray = GL_TEXTURE_CUBE_MAP_ARRAY,
		TextureRectangle = GL_TEXTURE_RECTANGLE
	};

	enum WrapMode : GLenum // Wrapper for wrap mode
	{
		ClampToEdge = GL_CLAMP_TO_EDGE,
		MirroredRepeat = GL_MIRRORED_REPEAT,
		Repeat = GL_REPEAT
	};

	enum Filter : GLenum // Wrapper for mag filter
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	};

	enum Format : GLenum // Compression format
	{
		R = GL_RED,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		HDR = GL_RGB16F
	};

protected:
	void Generate();
	virtual void Init();

private:
	static Texture* current;

	// Properties
	Type type = Type::Texture2D;
	WrapMode wrapMode = WrapMode::Repeat;
	Filter filter = Filter::Linear;
	
	GLint width, height, numOfChannels = 0;

public:
	
	// Properties
	bool bGenerateMipMaps = true;
	bool bFlipVertical = true;
	Format internalFormat = Format::RGB;
	Format format = Format::RGB;


	// Constructors & destructors
	Texture();
	Texture(const std::string& Filename);
	~Texture();


	// Methods
	void Bind() override;
	void Unbind() override;
	void SetType(const Type& TextureType);
	void SetWrapMode(const WrapMode& TextureWrapMode);
	void SetFilter(const Filter& TextureFilter);

	/* Import Texture asset from file */
	bool Import(const std::string& Filename) override;


	// Setters
	void SetFormat(const Format& NewFormat, const GLboolean& SetInternalFormat = GL_TRUE);
	void SetFormatHDR();

	// Getters
	GLboolean IsValid() const;
	Type GetType() const;
	WrapMode GetWrapMode() const;
	Filter GetFilter() const;
	GLint GetWidth() const;
	GLint GetHeight() const;
	glm::vec2 GetSize() const;
	GLint GetNumberOfChannels() const;


	// GUI
	void DrawWindow() override;
};




class Texture2D : public Texture
{
protected:
	Type type = Type::Texture2D;
};