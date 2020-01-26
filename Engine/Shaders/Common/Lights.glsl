#version 330 core

// ========================================= Lights =============================================

// Assume we have included "../outMaterials.glsl" - this will be added recursively soon
// We will  assume a variable called outMaterial already exists which is the result of our material shader

struct DirectionalLight
{
	vec3 Direction;
	vec3 Radiance;
};

vec3 BlinnPhong(vec3 Direction, vec3 Radiance)
{
	// Vectors
	vec3 L = normalize(Direction);
	vec3 N = normalize(PixelNormal);
	vec3 V = normalize(ViewDirection);
	
	// Blinn
	float NoV = max( dot(N, -L), 0.0f );
	vec3 diffuse = vec3(NoV) * Radiance * outMaterial.Albedo;
	
	// Phong
	vec3 reflectDir = reflect(-L, N);
	vec3 specular = pow( max(dot(V, reflectDir), 0.0f), 128.0f) * Radiance * 0.5f;
	
	return diffuse + specular;
}

vec3 DefaultShading(vec3 N, vec3 InTangent, vec3 V, vec3 L, vec3 Radiance, Material Mat)
{
	// Vectors
	L = -normalize(L);
	vec3 H = normalize(V + L); // Half vector between view direction and light direction
	float NoV = max(dot(N, V), 0.0f);
	float NoL = max(dot(N, L), 0.0f);
	float NoH = max(dot(N, H), 0.0f);
	float HoV = max(dot(H, V), 0.0f);
	// Anisotropic
	//InTangent = mul(LocalToWorld, vec4(1.0f, 0.0f, 0.0f, 1.0f)).xyz;
	InTangent = normalize(InTangent);
	vec3 Bitangent = cross(InTangent, N);

	vec3 T = normalize(mix(InTangent, Bitangent, Mat.AnisotropicDirection));
	vec3 B = normalize(mix(Bitangent, InTangent, Mat.AnisotropicDirection));
	float HoX = dot(H, T);
	float HoY = dot(H, B);
	
	vec3 F0 = vec3(0.04f); // Default 4% reflectivity
	F0 = mix(F0, Mat.Albedo, Mat.Metalness); //F0 packed into Albedo, blended by Metalness
	
	
	// Cook-torrance BRDF
	float NDF;
	if (Mat.Anisotropic > 0)
	{
		NDF = DistributionTrowbridgeReitzGGX(NoH, HoX, HoY, Mat.Roughness, Mat.Anisotropic);
	}
	else
	{
		NDF = DistributionGGX(NoH, Mat.Roughness);
	}
	float G = 	GeometrySmith(NoV, NoL, Mat.Roughness);
	vec3  F =	FresnelSchlick(HoV, F0, Mat.Roughness);
	
	
	vec3 kS = F;
	vec3 kD = vec3(1.0f) - kS;
	kD *= 1.0f - Mat.Metalness;
	
	
	vec3 numerator = NDF * G * F;
	float denominator = 4.0f * NoV * NoL;
	vec3 specular = numerator / max(denominator, 0.001f); // Avoid division by zero
	
	vec3 Lo = ((kD * Mat.Albedo / PI) + specular) * Radiance * NoL;
	return Lo;
}

vec3 CalculateDirectionalLight(DirectionalLight Light)
{
	//return BlinnPhong(Light.Direction, Light.Radiance);
	return DefaultShading(PixelNormal, VertexTangent, ViewDirection, Light.Direction, Light.Radiance, outMaterial);
}

struct PointLight
{
	vec3 Position;
	vec3 Radiance;
};

vec3 CalculatePointLight(PointLight Light)
{
	vec3 dir = WorldPosition - Light.Position;
	float distance = length(dir);
	float attenuation = 1.0f / (distance*distance); // Inverse squared attenuations
	
	//return BlinnPhong(dir, Light.Radiance * attenuation);
	return DefaultShading(PixelNormal, VertexTangent, ViewDirection, dir, Light.Radiance * attenuation, outMaterial);
}

struct SpotLight
{
	vec3 Position;
	vec3 Direction;
	vec3 Radiance;
	float CosAngle;
};

vec3 CalculateSpotLight(SpotLight Light)
{
	vec3 dir = WorldPosition - Light.Position;
	vec3 L = normalize(-Light.Direction);
	
	float cosTheta = dot(normalize(dir), L);
	if (cosTheta > Light.CosAngle)
	{
		float distance = length(dir);
		float attenuation = 1.0f / (distance*distance); // Inverse squared attenuations
		//return BlinnPhong(normalize(dir), Light.Radiance * attenuation);
		return DefaultShading(PixelNormal, VertexTangent, ViewDirection, dir, Light.Radiance * attenuation, outMaterial);
	}
	else
		return vec3(0.0f);
}