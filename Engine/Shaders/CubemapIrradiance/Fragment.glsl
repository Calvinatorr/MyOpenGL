#version 330 core
out vec4 FragColour; // Output

in vec3 LocalPosition;


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
	
	
	vec3 irradiance = vec3(0.0f);
	
	vec3 N = normalize(LocalPosition.xyz);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = cross(up, N);
	up = cross(N ,right);
	
	float SampleDelta = 0.1f;
	float accumulatedSamples = 0.0f;
	for (float phi = 0.0f; phi < 2.0f * PI; phi += SampleDelta)
	{
		for (float theta = 0.0f; theta < 0.5f * PI; theta += SampleDelta)
		{
			float SIN_THETA = sin(theta);
			float COS_THETA = cos(theta);
			float SIN_PHI = sin(phi);
			float COS_PHI = cos(phi);
			
			// Spherical to cartesian (in tangent space)
			vec3 tangentSample = vec3(SIN_THETA * COS_PHI, SIN_THETA * SIN_PHI, COS_THETA);
			// Tangent space to world
			vec3 sample = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
			
			irradiance += texture(EnvironmentMap, sample).rgb * COS_THETA * SIN_THETA;
			++accumulatedSamples;
		}
	}
	irradiance = PI * irradiance * (1.0f / float(accumulatedSamples));
	
	//envColour = envColour / (envColour + vec3(1.0f));
	//envColour = pow(envColour, vec3(1.0f / 2.2f));
	
	FragColour = vec4(irradiance, 1.0f);
}