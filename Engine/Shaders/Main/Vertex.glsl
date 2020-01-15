#version 330 core
layout (location = 0) in vec3 aPos; 	 // Input vertex position
layout (location = 1) in vec2 aTexCoord; // Input texture coordinates
layout (location = 2) in vec3 aColour; 	 // Input vertex colour
layout (location = 3) in vec3 aNormal;	 // Input normal


// Uniforms
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
#include "Common/Common.glsl"


// Outputs
out vec2 TexCoord;
out vec3 VertexColour;
out vec3 VertexNormal;

out mat4 LocalToWorld;
out vec3 LocalPosition;
out vec3 WorldPosition;


void main()
{
	float angle = -ElapsedTime * .5f;
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	mat4 rot = mat4(
		cosAngle, 0.0f, sinAngle, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinAngle, 0.0f, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	/*mat4 rot = mat4(
		cosAngle, -sinAngle, 0.0f, 0.0f,
		sinAngle, cosAngle, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);*/
	
	mat4 NewModel = Model;
	//NewModel *= rot;
	mat4 Transform = Projection * View * NewModel;
	
	LocalToWorld = transpose(inverse(NewModel)); // Matrix to transform for local space to world space, includes shader based transforms
	LocalPosition = aPos;
	WorldPosition = vec3(NewModel * vec4(aPos, 1.0f));
	
	TexCoord = aTexCoord;	// Pass through TexCoords
	VertexColour = aColour; // Pass through vertex colour
	VertexNormal = normalize(vec3(LocalToWorld * vec4(aNormal, 1.0f))); // Vertex normal to world space

	gl_Position = Transform * vec4(aPos, 1.0f);
	gl_Position.y += abs(sin(ElapsedTime)) * 0.5f;
	//gl_Position.y += sin(ElapsedTime);
    //gl_Position = vec4(aPos.x, aPos.y + sin(ElapsedTime) * .1, aPos.z, 1.0);
}