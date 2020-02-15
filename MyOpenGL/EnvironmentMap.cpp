#include "EnvironmentMap.h"
#pragma once


void IrradianceMap::ProcessIrradiance()
{
	// Initialize convolution shader (do once)
	static Shader irradianceShader;
	static bool bInitialized = false;
	if (!bInitialized)
	{
		irradianceShader.Import(SHADER_PATH + "CubemapIrradiance");
		bInitialized = true;
	}

	ApplyShader(&irradianceShader, this, 32);
}


bool IrradianceMap::Import(const std::string & Filename)
{
	bool bSuccess = true;
	bSuccess = TextureCube::Import(Filename);
	ProcessIrradiance();
	return bSuccess;
}


bool EnvironmentMap::Import(const std::string & Filename)
{
	bool bSuccess = true;
	bSuccess = irradianceMap.Import(Filename);
	return bSuccess;
}