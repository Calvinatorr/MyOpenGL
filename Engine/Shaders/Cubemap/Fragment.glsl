#version 330 core
out vec4 FragColour; // Output

// Inputs marshalled through vertex shader
in vec2 TexCoord;
in vec4 VertexColour;
in vec3 VertexNormal;

in mat4 LocalToWorld;
in vec3 LocalPosition;
in vec3 WorldPosition;


// Uniforms
#include "Common/Common.glsl"
#include "Common/CommonFragment.glsl"

// Samplers
uniform sampler2D EquirectangularMap;

#include "Common/Material.glsl"

// ========================================= MAIN RENDER =============================================

void main()
{
	outMaterial = inMaterial;
	
	vec2 uv = SphericalUVsFromPosition(normalize(LocalPosition));
	
	FragColour = texture(EquirectangularMap, uv.xy);
	//FragColour = vec4(.5f);
} 