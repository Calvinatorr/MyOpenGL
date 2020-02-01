#version 330 core
out vec4 FragColour; 


in vec2 TexCoord;
in vec4 VertexColour;
in vec3 VertexNormal;

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

#define saturate(x) clamp(x, 0.0f, 1.0f)
#define sqr(x)



vec3 PixelNormal = normalize(VertexNormal); 
/*#ifdef VertexTangent
vec3 PixelTangent = normalize(VertexTangent);
vec3 PixelBitangent = normalize(cross(PixelNormal, PixelTangent));
#endif*/

vec3 ViewDirection = normalize(CameraPosition - WorldPosition);

vec3 GetCustomReflectionVector(vec3 WorldNormal)
{
	return -ViewDirection + WorldNormal * dot(WorldNormal, ViewDirection) * 2.0f;
}

vec3 ReflectionVector = GetCustomReflectionVector(PixelNormal);


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

void ClampMaterialProperties(in out Material InMat)
{
	InMat.Albedo = clamp(InMat.Albedo, 0.0f, 1.0f);
	InMat.Metalness = clamp(InMat.Metalness, 0.0f, 1.0f);
	InMat.Roughness = clamp(InMat.Roughness, 0.0f, 1.0f);
	InMat.Anisotropic = clamp(InMat.Anisotropic, 0.0f, 1.0f);
	InMat.AnisotropicDirection = clamp(InMat.AnisotropicDirection, 0.0f, 1.0f);
	InMat.AmbientOcclusion = clamp(InMat.AmbientOcclusion, 0.0f, 1.0f);
}




void main()
{
	outMaterial = inMaterial;
	outMaterial.Albedo = vec3(0.8f);
	vec3 colour = outMaterial.Albedo;
	
	
	colour = colour / (colour + vec3(1.0f));
	colour = pow(colour, vec3(1.0f / 2.2f));

	FragColour = vec4(colour, 1.0f);
} 
