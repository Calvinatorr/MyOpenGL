#version 330 core


// ========================================= NDFs =============================================

float DistributionGGX(float NoH, float Roughness)
{
	float a = Roughness * Roughness;
	float a2 = a * a;
	float NoH2 = NoH * NoH;
	
	float num = a2;
	//float denom = NoH2 * (a2 - 1.0f) + 1.0f;
	float denom = ( NoH * a2 - NoH ) * NoH + 1;	// 2 mad
	denom = PI * denom * denom;
	
	return num / denom;
}


float DistributionTrowbridgeReitzGGX(float NoH, float HoX, float HoY, float Roughness, float Anisotropic)
{
	float a = Roughness*Roughness;
	float a2 = a*a;
	float aspect = sqrt(1.0f - Anisotropic * 0.9f);
	float X = max(0.001f, a2 / aspect) * 5.0f;
	float Y = max(0.001f, a2 * aspect) * 5.0f;

	return 1.0f / (PI * X*Y * pow(pow(HoX/X, 2) + pow(HoY/Y, 2) + NoH*NoH, 2));
}


float GeometrySchlickGGX(float NoV, float Roughness)
{
	float r = Roughness + 1.0f;
	float k = (r * r) / 8.0f;
	
	float num = NoV;
	float denom = NoV * (1.0f - k) + k;
	
	return num / denom;
}

float GeometrySmith(float NoV, float NoL, float Roughness)
{
	float ggx2 = GeometrySchlickGGX(NoV, Roughness);
	float ggx1 = GeometrySchlickGGX(NoL, Roughness);
	
	return ggx1 * ggx2;
}

vec3 FresnelSchlick(float CosTheta, vec3 F0, float Roughness)
{
	return F0 + (1.0f - F0) * pow(1.0f - CosTheta, 5.0f);
}