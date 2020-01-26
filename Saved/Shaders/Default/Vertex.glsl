#version 330 core
layout (location = 0) in vec3 aPos; 	 




uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;




out mat4 LocalToWorld;
out vec3 LocalPosition;
out vec3 WorldPosition;


void main()
{
	mat4 NewModel = Model;
	mat4 Transform = Projection * View * NewModel;
	
	LocalToWorld = transpose(inverse(NewModel)); 
	LocalPosition = aPos;
	WorldPosition = vec3(NewModel * vec4(aPos, 1.0f));

	gl_Position = Transform * vec4(aPos, 1.0f);
}
