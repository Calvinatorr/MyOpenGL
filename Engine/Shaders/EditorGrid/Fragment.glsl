#version 330 core
out vec4 FragColour; // Output

// Inputs marshalled through vertex shader
in vec2 TexCoord;
in vec3 VertexNormal;

in mat4 LocalToWorld;
in vec3 LocalPosition;
in vec3 WorldPosition;


// Uniforms
#include "Common/Common.glsl"
#include "Common/CommonFragment.glsl"


#include "Common/Material.glsl"
#include "Common/Tonemapping.glsl"


float GenerateGrid(vec2 InTiling, float InBorderSize)
{
	vec2 uv = (TexCoord.xy * InTiling) - .5f;
	uv = fract(uv);
	uv = (uv - .5f) * 2.0f;
	uv = abs(uv);
	
	float x = StepAA(uv.x, InBorderSize);
	float y = StepAA(uv.y, InBorderSize);
	float grid = max(x, y);
	
	return grid;
}


// ========================================= MAIN RENDER =============================================

void main()
{
	vec2 LargeGridTiling = vec2(10.0f, 10.0f);
	float LargeGridBorderSize = .025f;
	float largeGrid = GenerateGrid(LargeGridTiling, LargeGridBorderSize);
	
	vec2 SmallGridTiling = vec2(100.0f, 100.0f);
	float SmallGridBorderSize = .05f;
	float smallGrid = GenerateGrid(SmallGridTiling, SmallGridBorderSize);
	
	float grid = max(largeGrid, smallGrid * .15f);
	
	if (grid < .0001f)
		discard;
		
		
	
		
	vec3 worldPos = saturate(abs(WorldPosition));
	worldPos = vec3(1.0f)-worldPos;
	worldPos.xz = worldPos.zx; // Swizzle so we point in direction of axis
	worldPos.y = 0.0f;
	worldPos = max(worldPos, .15f);
	
	vec3 GridColour = worldPos * vec3(0.3f);
	FragColour = vec4(TonemapSCurve_ACES(GridColour), 1.0f);
} 