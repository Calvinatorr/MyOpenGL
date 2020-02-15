#version 330 core
out vec4 FragColor; 

in vec3 LocalPosition;






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



uniform sampler2D MapToProcess;




void main()
{
	FragColor = vec4(SampleCubemap(MapToProcess, normalize(LocalPosition).xyz).xyz, 1.0f);
} 