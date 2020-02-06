#pragma once

#include "Texture.h"
#include "Shader.h"
#include "Primitives.h"


class Shader;
class Texture;
class TextureCube;


class TextureCube : public Texture
{
private:
	static Shader equirectangularToCubemapShader;
	static bool bInitializedEquirectangularToCubemapShader;
	static void InitializeEquirectangularToCubemapShader();

protected:
	Type type = Type::TextureCube;
	WrapMode wrapMode = WrapMode::ClampToEdge;


public:

	TextureCube() : Texture() {};

	Format format = Format::RGB;
	Format interlaFormat = Format::HDR;

	void ConvertTexture2DToCubemap();

};