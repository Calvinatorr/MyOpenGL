#version 330 core

uniform samplerCube IrradianceMap;
uniform samplerCube ReflectionMap;

vec3 GetIrradianceIBL(vec3 WorldPosition)
{
	return texture(IrradianceMap, normalize(WorldPosition)).rgb;
}