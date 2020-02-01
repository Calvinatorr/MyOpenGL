#pragma once
#include "Texture.h"

class Texture2D : public Texture
{
protected:
	Texture::Type type = Texture::Texture2D;

public:
	Texture2D();
	~Texture2D();
};

