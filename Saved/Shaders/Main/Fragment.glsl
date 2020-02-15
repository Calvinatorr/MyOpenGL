#version 330 core
out vec4 FragColour; 



in vec2 TexCoord;
in vec4 VertexColour;
in vec3 VertexNormal;
in vec3 VertexTangent;

in mat4 LocalToWorld;
in vec3 LocalPosition;
in vec3 WorldPosition;




uniform vec3 MinBounds;
uniform vec3 MaxBounds;
vec3 Size = MaxBounds - MinBounds;
uniform vec3 CameraPosition;
uniform vec3 CameraDirection;
uniform float ElapsedTime;



#define PI 3.14159265359
#define INV_PI 0.31830988618
#define DEG_TO_RAD 0.0174533
#define RAD_TO_DEG 57.2958

#define saturate(x) clamp(x, 0.0f, 1.0f)
#define sqr(x)



vec3 GetLocalUVW(vec3 LocalPosition)
{
	return (LocalPosition - MinBounds) / Size;
}



const vec2 INV_ATAN = vec2(0.1591, 0.3183);

vec2 SphericalUVsFromPosition(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= INV_ATAN;
	uv += 0.5f;
	return uv;
}

vec4 SampleCubemap(sampler2D TexCube, vec3 Position)
{
	vec2 uv = SphericalUVsFromPosition(Position);
	return texture(TexCube, uv.xy);
}



/* Transforms vector by matrix */
vec3 TransformVector(vec3 InVector, mat4 Matrix)
{
	return mul(vec4(InVector, 0.0f), Matrix).xyz;
}
/* Transforms position by matrix */
vec3 TransformPosition(vec3 InPosition, mat4 Matrix)
{
	return mul(vec4(InPosition, 1.0f), Matrix).xyz;
}



vec3 PixelNormal = normalize(VertexNormal); 
/*#ifdef VertexTangent
vec3 PixelTangent = normalize(VertexTangent);
vec3 PixelBitangent = normalize(cross(PixelNormal, PixelTangent));
#endif*/

vec3 ViewDirection = normalize(CameraPosition - WorldPosition);


/* Cubemaps */
vec3 GetCustomReflectionVector(vec3 WorldNormal)
{
	return -ViewDirection + WorldNormal * dot(WorldNormal, ViewDirection) * 2.0f;
}
vec3 ReflectionVector = GetCustomReflectionVector(PixelNormal);
vec4 SampleCubemapAsReflection(sampler2D TexCube)
{
	return SampleCubemap(TexCube, normalize(ReflectionVector));
}



/* Smooth step using inverse lerp. Only runs in pixel shader due to using fwidth() (ddx() & ddy()) */ 
float StepAA(float In, float Gradient)
{
	float halfChange = fwidth(Gradient) / 2;
	
	float lowerEdge = In - halfChange;
	float upperEdge = In + halfChange;
	
	float stepped = (Gradient - lowerEdge) / (upperEdge - lowerEdge);
	stepped = saturate(stepped);
	return stepped;
}


uniform sampler2D tex;
uniform sampler2D tex2;



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

vec3 GetF0(vec3 Albedo, float Metalness)
{
	return mix(vec3(0.04f), Albedo.rgb, Metalness.r).rgb;
}

void ClampMaterialProperties(in out Material InMat)
{
	InMat.Albedo = clamp(InMat.Albedo, 0.0f, 1.0f);
	InMat.Metalness = clamp(InMat.Metalness, 0.0f, 1.0f);
	InMat.Roughness = clamp(InMat.Roughness, 0.0f, 1.0f);
	InMat.Anisotropic = clamp(InMat.Anisotropic, 0.0f, 1.0f);
	InMat.AnisotropicDirection = clamp(InMat.AnisotropicDirection, 0.0f, 1.0f);
	InMat.AmbientOcclusion = clamp(InMat.AmbientOcclusion, 0.0f, 1.0f);
}





float DistributionGGX(float NoH, float Roughness)
{
	float a = Roughness * Roughness;
	float a2 = a * a;
	float NoH2 = NoH * NoH;
	
	float num = a2;
	
	float denom = ( NoH * a2 - NoH ) * NoH + 1;	
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


vec3 FresnelSchlick(float CosTheta, vec3 F0)
{
    return F0 + (1.0f - F0) * pow(1.0f - CosTheta, 5.0f);
}


vec3 FresnelSchlickRoughness(float CosTheta, vec3 F0, float Roughness)
{
	return F0 + (max(vec3(1.0f - Roughness), F0) - F0) * pow(1.0f - CosTheta, 5.0f);
}


uniform samplerCube IrradianceMap;
uniform samplerCube ReflectionMap;

vec3 GetIrradianceIBL(vec3 WorldPosition)
{
	return texture(IrradianceMap, normalize(WorldPosition)).rgb;
}







struct DirectionalLight
{
	vec3 Direction;
	vec3 Radiance;
};

vec3 BlinnPhong(vec3 Direction, vec3 Radiance)
{
	
	vec3 L = normalize(Direction);
	vec3 N = normalize(PixelNormal);
	vec3 V = normalize(ViewDirection);
	
	
	float NoV = max( dot(N, -L), 0.0f );
	vec3 diffuse = vec3(NoV) * Radiance * outMaterial.Albedo;
	
	
	vec3 reflectDir = reflect(-L, N);
	vec3 specular = pow( max(dot(V, reflectDir), 0.0f), 128.0f) * Radiance * 0.5f;
	
	return diffuse + specular;
}

vec3 DefaultShading(vec3 N, vec3 InTangent, vec3 V, vec3 L, vec3 Radiance, Material Mat)
{
	
	L = -normalize(L);
	vec3 H = normalize(V + L); 
	float NoV = max(dot(N, V), 0.0f);
	float NoL = max(dot(N, L), 0.0f);
	float NoH = max(dot(N, H), 0.0f);
	float HoV = max(dot(H, V), 0.0f);
	
	
	InTangent = normalize(InTangent);
	vec3 Bitangent = cross(InTangent, N);

	vec3 T = normalize(mix(InTangent, Bitangent, Mat.AnisotropicDirection));
	vec3 B = normalize(mix(Bitangent, InTangent, Mat.AnisotropicDirection));
	float HoX = dot(H, T);
	float HoY = dot(H, B);
	
	vec3 F0 = vec3(0.04f); 
	F0 = mix(F0, Mat.Albedo, Mat.Metalness); 
	
	
	
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
	vec3  F =	FresnelSchlickRoughness(HoV, F0, Mat.Roughness);
	
	
	vec3 kS = F;
	vec3 kD = vec3(1.0f) - kS;
	kD *= 1.0f - Mat.Metalness;
	
	
	vec3 numerator = NDF * G * F;
	float denominator = 4.0f * NoV * NoL + 0.001f;
	vec3 specular = numerator / max(denominator, 0.001f); 
	
	vec3 Lo = ((kD * Mat.Albedo / PI) + specular) * Radiance * NoL;
	return Lo;
}

vec3 CalculateDirectionalLight(DirectionalLight Light)
{
	
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
	float attenuation = 1.0f / (distance*distance); 
	
	
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
		float attenuation = 1.0f / (distance*distance); 
		
		return DefaultShading(PixelNormal, VertexTangent, ViewDirection, dir, Light.Radiance * attenuation, outMaterial);
	}
	else
		return vec3(0.0f);
}





vec3 TonemapSCurve_ACES(vec3 x)
{
  	float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e),0.0,1.0);
}




#define NUM_OF_LIGHTS 4
PointLight lights[NUM_OF_LIGHTS];



void main()
{
	
	outMaterial = inMaterial;
	vec4 t = texture(tex, TexCoord.xy);
	vec4 t2 = texture(tex2, TexCoord.xy);
	
	
	
	
		
	
	
	ClampMaterialProperties(outMaterial);
	
	
	
	
	
	
	
	
	
	vec3 Lo = vec3(0.0f); 
	
	
	DirectionalLight dirLight;
	dirLight.Radiance = vec3(1.0f * PI);
	dirLight.Direction = normalize(vec3(-.333f, -.3333f, -.333f));
	float angle = ElapsedTime * .5f;
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	mat4 rot = mat4(
		cosAngle, 0.0f, sinAngle, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinAngle, 0.0f, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	dirLight.Direction = vec3(rot * vec4(dirLight.Direction, 1.0f));
	Lo += CalculateDirectionalLight(dirLight);
	
	/*for (int i = 0; i < NUM_OF_LIGHTS; i++)
	{
		float a = float(i) / float(NUM_OF_LIGHTS);
		a *= 2.0f * PI;
		const float freq = 5.0f;
		lights[i].Radiance = vec3(20.0f);
		lights[i].Position = vec3(sin(a) * freq, 5.0f, cos(a) * freq);
		Lo += CalculatePointLight(lights[i]);
	}/*
	
	/*SpotLight spotLight;
	spotLight.Radiance = vec3(32.0f);
	spotLight.Position = vec3(0.0f, 2.5f, 10.0f);
	spotLight.Direction = vec3(0.0f, .3333f, 1.0f);
	spotLight.CosAngle = cos(12.5 * DEG_TO_RAD);
	Lo += CalculateSpotLight(spotLight);*/


	
	vec3 F0 = GetF0(outMaterial.Albedo, outMaterial.Metalness);
	vec3 kS = FresnelSchlickRoughness(max(dot(PixelNormal, ViewDirection), 0.0f), F0, outMaterial.Roughness);
	vec3 kD = 1.0f - kS;
	kD *= 1.0f - outMaterial.Metalness;
	vec3 irradiance = GetIrradianceIBL(WorldPosition);
	vec3 diffuse = irradiance * outMaterial.Albedo;
	vec3 ambient = (kD * diffuse) * outMaterial.AmbientOcclusion;	
	
	vec3 colour = ambient + Lo;
	
	
	
	
	
	
	colour = TonemapSCurve_ACES(colour);
	FragColour = vec4(colour, 1.0f);
} 
