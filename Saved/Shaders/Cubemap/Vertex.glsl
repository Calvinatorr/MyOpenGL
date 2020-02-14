#version 330 core
layout (location = 0) in vec3 aPos; 	 
layout (location = 1) in vec2 aTexCoord; 



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

out vec2 TexCoord;


out vec3 LocalPosition;


void main()
{
	LocalPosition = aPos;
	
	mat4 rotView = mat4(mat3(View));
	vec4 clipPos = Projection * rotView * vec4(LocalPosition, 1.0f);
	TexCoord = aTexCoord;

	gl_Position = clipPos.xyww;
}
