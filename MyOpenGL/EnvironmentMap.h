#pragma once

#include "TextureCube.h"
#include "Shader.h"


class IrradianceMap : public TextureCube
{
protected:
	void ProcessIrradiance();

public:

	virtual bool Import(const std::string& Filename) override;
};


class EnvironmentMap : public Asset
{
public:
	
	TextureCube irradianceMap, reflectionMap;

	virtual bool Import(const std::string& Filename) override;
	//void Bind() override;
};