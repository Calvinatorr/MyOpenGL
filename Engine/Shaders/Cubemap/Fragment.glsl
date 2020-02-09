#version 330 core
out vec4 FragColour; // Output

in vec3 LocalPosition;
in vec2 TexCoord;


// Uniforms
#include "Common/Common.glsl"
//#include "Common/CommonFragment.glsl"

// Samplers
uniform samplerCube EnvironmentMap;
//uniform sampler2D EnvironmentMap;


// ========================================= MAIN RENDER =============================================

void main()
{
	vec3 envColour = vec3(1.0f, 0.0f, 0.0f);
	envColour = texture(EnvironmentMap, LocalPosition.xyz).rgb;
	
	envColour = envColour / (envColour + vec3(1.0f));
	envColour = pow(envColour, vec3(1.0f / 2.2f));
	
	FragColour = vec4(envColour, 1.0f);
}