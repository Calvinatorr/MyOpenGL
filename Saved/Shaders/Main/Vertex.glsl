#version 330 core
layout (location = 0) in vec3 aPos; 	 
layout (location = 1) in vec2 aTexCoord; 
layout (location = 2) in vec4 aColour; 	 
layout (location = 3) in vec3 aNormal;	 
layout (location = 4) in vec3 aTangent;	 



uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;



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



out vec2 TexCoord;
out vec4 VertexColour;
out vec3 VertexNormal;
out vec3 VertexTangent;

out mat4 LocalToWorld;
out vec3 LocalPosition;
out vec3 WorldPosition;


void main()
{
	float angle = -ElapsedTime * .5f;
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	mat4 rot = mat4(
		cosAngle, 0.0f, sinAngle, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinAngle, 0.0f, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	/*mat4 rot = mat4(
		cosAngle, -sinAngle, 0.0f, 0.0f,
		sinAngle, cosAngle, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);*/
	
	mat4 NewModel = Model;
	
	mat4 Transform = Projection * View * NewModel;
	
	LocalToWorld = transpose(inverse(NewModel)); 
	LocalPosition = aPos;
	WorldPosition = vec3(NewModel * vec4(aPos, 1.0f));
	
	TexCoord = aTexCoord;	
	VertexColour = aColour; 
	VertexNormal = normalize(vec3(LocalToWorld * vec4(aNormal, 1.0f))); 
	VertexTangent = normalize(vec3(LocalToWorld * vec4(aTangent, 1.0f))); 

	gl_Position = Transform * vec4(aPos, 1.0f);
	gl_Position.y += abs(sin(ElapsedTime)) * 0.5f;
	
    
}
