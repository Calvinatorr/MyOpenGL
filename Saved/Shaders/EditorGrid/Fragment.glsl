#version 330 core
out vec4 FragColour; 


in vec2 TexCoord;
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



vec3 TonemapSCurve_ACES(vec3 x)
{
  	float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e),0.0,1.0);
}


float GenerateGrid(vec2 InTiling, float InBorderSize)
{
	vec2 uv = (TexCoord.xy * InTiling) - .5f;
	uv = fract(uv);
	uv = (uv - .5f) * 2.0f;
	uv = abs(uv);
	
	float x = StepAA(uv.x, InBorderSize);
	float y = StepAA(uv.y, InBorderSize);
	float grid = max(x, y);
	
	return grid;
}




void main()
{
	vec2 LargeGridTiling = vec2(10.0f, 10.0f);
	float LargeGridBorderSize = .025f;
	float largeGrid = GenerateGrid(LargeGridTiling, LargeGridBorderSize);
	
	vec2 SmallGridTiling = vec2(100.0f, 100.0f);
	float SmallGridBorderSize = .05f;
	float smallGrid = GenerateGrid(SmallGridTiling, SmallGridBorderSize);
	
	float grid = max(largeGrid, smallGrid * .15f);
	
	if (grid < .0001f)
		discard;
		
		
	
		
	vec3 worldPos = saturate(abs(WorldPosition));
	worldPos = vec3(1.0f)-worldPos;
	worldPos.xz = worldPos.zx; 
	worldPos.y = 0.0f;
	worldPos = max(worldPos, .15f);
	
	vec3 GridColour = worldPos * vec3(0.3f);
	FragColour = vec4(TonemapSCurve_ACES(GridColour), 1.0f);
} 
