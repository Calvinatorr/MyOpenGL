#version 330 core
out vec4 FragColor; // Output

in vec3 LocalPosition;


// Uniforms
#include "Common/Common.glsl"
//#include "Common/CommonFragment.glsl"

// Samplers
uniform sampler2D EquirectangularMap;


// ========================================= MAIN RENDER =============================================

void main()
{
	FragColor = vec4(SampleCubemap(EquirectangularMap, normalize(LocalPosition).xyz).xyz, 1.0f);
} 