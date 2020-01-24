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


vec3 PixelNormal = normalize(VertexNormal); 
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


uniform sampler2D EquirectangularMap;



struct Material
{
	vec3 Albedo;
	float Metalness;
	float Roughness;
	float AmbientOcclusion;
};

uniform Material inMaterial;
Material outMaterial;



void main()
{
	outMaterial = inMaterial;
	
	vec2 uv = SphericalUVsFromPosition(normalize(LocalPosition));
	
	FragColour = texture(EquirectangularMap, uv.xy);
	
} 
