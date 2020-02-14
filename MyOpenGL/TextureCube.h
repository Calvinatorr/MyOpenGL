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
	int targetResolution = 512;
	Texture equirectangularMap;

public:

	TextureCube();

	virtual bool Import(const std::string& Filename) override;

	/* Get equirectangular map used to generate the cubemap */
	Texture& const GetEquirectangularMap();

	/* Return the equirectangular map ID for previewing */
	GLint GetPreviewID() override;


	void DrawWindowContents() override;
};