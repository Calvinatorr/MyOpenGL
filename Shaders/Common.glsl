#version 330 core

// Uniforms
uniform vec3 MinBounds;
uniform vec3 MaxBounds;
vec3 Size = MaxBounds - MinBounds;
uniform vec3 CameraPosition;
uniform vec3 CameraDirection;
uniform float ElapsedTime;


// Constants
#define PI 3.14159265359
#define INV_PI 0.31830988618
#define DEG_TO_RAD 0.0174533
#define RAD_TO_DEG 57.2958


// Functions
vec3 GetLocalUVW(vec3 LocalPosition)
{
	return (LocalPosition - MinBounds) / Size;
}