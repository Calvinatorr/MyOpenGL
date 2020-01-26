#version 330 core

struct Material
{
	vec3 Albedo;
	float Metalness;
	float Roughness;
	float Anisotropic;
	float AnisotropicDirection;
	float AmbientOcclusion;
};

uniform Material inMaterial;
Material outMaterial;

void ClampMaterialProperties(in out Material InMat)
{
	InMat.Albedo = clamp(InMat.Albedo, 0.0f, 1.0f);
	InMat.Metalness = clamp(InMat.Metalness, 0.0f, 1.0f);
	InMat.Roughness = clamp(InMat.Roughness, 0.0f, 1.0f);
	InMat.Anisotropic = clamp(InMat.Anisotropic, 0.0f, 1.0f);
	InMat.AnisotropicDirection = clamp(InMat.AnisotropicDirection, 0.0f, 1.0f);
	InMat.AmbientOcclusion = clamp(InMat.AmbientOcclusion, 0.0f, 1.0f);
}