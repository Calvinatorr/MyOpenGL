#version 330 core
layout (location = 0) in vec3 aPos; 	 // Input vertex position
layout (location = 1) in vec2 aTexCoord; // Input texture coordinates


// Uniforms
uniform mat4 View;
uniform mat4 Projection;
#include "Common/Common.glsl"

out vec2 TexCoord;

// Outputs
out vec3 LocalPosition;


void main()
{
	LocalPosition = aPos;
	
	mat4 rotView = mat4(mat3(View));
	vec4 clipPos = Projection * rotView * vec4(LocalPosition, 1.0f);
	TexCoord = aTexCoord;

	gl_Position = clipPos.xyww;
}