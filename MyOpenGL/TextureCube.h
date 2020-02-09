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
	static void InitializeEquirectangularToCubemapShader();

public:

	TextureCube();

	virtual bool Import(const std::string& Filename) override;
};