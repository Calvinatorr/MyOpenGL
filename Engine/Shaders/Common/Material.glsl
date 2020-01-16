#version 330 core

struct Material
{
	vec3 Albedo;
	float Metalness;
	float Roughness;
	float AmbientOcclusion;
};

uniform Material inMaterial;
Material outMaterial;