#version 330 core
layout (location = 0) in vec3 aPos; 	 // Input vertex position


// Uniforms
uniform mat4 View;
uniform mat4 Projection;
#include "Common/Common.glsl"


// Outputs
out vec3 LocalPosition;


void main()
{
	LocalPosition = aPos;
	
	mat4 rotView = mat4(mat3(View));
	vec4 clipPos = Projection * rotView * vec4(LocalPosition, 1.0f);

	gl_Position = clipPos.xyww;
}