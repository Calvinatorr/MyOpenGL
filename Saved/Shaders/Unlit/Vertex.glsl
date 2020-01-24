#version 330 core
layout (location = 0) in vec3 aPos; 	 
layout (location = 1) in vec2 aTexCoord; 
layout (location = 2) in vec4 aColour; 	 
layout (location = 3) in vec3 aNormal;	 



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



out vec2 TexCoord;
out vec4 VertexColour;
out vec3 VertexNormal;

out mat4 LocalToWorld;
out vec3 LocalPosition;
out vec3 WorldPosition;


void main()
{
	mat4 NewModel = Model;
	mat4 Transform = Projection * View * NewModel;
	
	LocalToWorld = transpose(inverse(NewModel)); 
	LocalPosition = aPos;
	WorldPosition = vec3(NewModel * vec4(aPos, 1.0f));
	
	TexCoord = aTexCoord;	
	VertexColour = aColour; 
	VertexNormal = normalize(vec3(LocalToWorld * vec4(aNormal, 1.0f))); 

	gl_Position = Transform * vec4(aPos, 1.0f);
}
