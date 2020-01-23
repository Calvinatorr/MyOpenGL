#version 330 core
out vec4 FragColour; // Output

// Uniforms
// Inputs marshalled through vertex shader
in vec2 TexCoord;
in vec4 VertexColour;
in vec3 VertexNormal;

in mat4 LocalToWorld;
in vec3 LocalPosition;
in vec3 WorldPosition;

#include "Common/Common.glsl"
#include "Common/CommonFragment.glsl"

// Samplers
uniform sampler2D tex;
uniform sampler2D tex2;

#include "Common/Material.glsl"
#include "Common/BRDFs.glsl"
#include "Common/Lights.glsl"


#include "Common/Tonemapping.glsl"


// ========================================= LIGHTS =============================================

#define NUM_OF_LIGHTS 4
PointLight lights[NUM_OF_LIGHTS];

// ========================================= MAIN RENDER =============================================

void main()
{
	// Materials
	outMaterial = inMaterial;
	vec4 t = texture(tex, TexCoord.xy);
	vec4 t2 = texture(tex2, TexCoord.xy);
	//outMaterial.Albedo *= vec3( mix( t, t2, t2.a ) );
	
	//outMaterial.Albedo *= vec3(t2);
	//outMaterial.Albedo = vec3(.8f);
		
	//vec3 localUVW = GetLocalUVW(LocalPosition);
	//outMaterial.Roughness = localUVW.r;
	outMaterial.AmbientOcclusion = clamp(outMaterial.AmbientOcclusion, 0.0f, 1.0f);
	outMaterial.Metalness = clamp(outMaterial.Metalness, 0.0f, 1.0f);
	outMaterial.Roughness = clamp(outMaterial.Roughness, 0.0f, 1.0f);
	outMaterial.Albedo = clamp(outMaterial.Albedo, 0.0f, 1.0f);
	
	//outMaterial.Albedo = vec3(mix(t, t2, t2.a));
	//outMaterial.Albedo = vec3(1.0f, 0.0f, 0.0f);
	//outMaterial.Albedo = vec3(1.0f);
	//outMaterial.Metalness = localUVW.x;
	//outMaterial.Roughness = .2f; //localUVW.z;
	//outMaterial.AmbientOcclusion = 1.0f;
	
	
	vec3 Lo = vec3(0.0f); // Lobe
	
	// Lights
	DirectionalLight dirLight;
	dirLight.Radiance = vec3(1.0f);
	dirLight.Direction = normalize(vec3(-.333f, -.3333f, -.333f));
	float angle = ElapsedTime * .5f;
	float cosAngle = cos(angle);
	float sinAngle = sin(angle);
	mat4 rot = mat4(
		cosAngle, 0.0f, sinAngle, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sinAngle, 0.0f, cosAngle, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	dirLight.Direction = vec3(rot * vec4(dirLight.Direction, 1.0f));
	//Lo += CalculateDirectionalLight(dirLight);
	
	PointLight pointLight;
	pointLight.Radiance = vec3(3.142f * 2.0f);
	pointLight.Position = vec3(0.0f, sin(ElapsedTime * 2.0f) * 1.5f, 0.0f);
	//Lo += CalculatePointLight(pointLight);
	
	for (int i = 0; i < NUM_OF_LIGHTS; i++)
	{
		float a = float(i) / float(NUM_OF_LIGHTS);
		a *= 2.0f * PI;
		const float freq = 5.0f;
		lights[i].Radiance = vec3(20.0f);
		lights[i].Position = vec3(sin(a) * freq, 5.0f, cos(a) * freq);
		Lo += CalculatePointLight(lights[i]);
	}
	
	SpotLight spotLight;
	spotLight.Radiance = vec3(32.0f);
	spotLight.Position = vec3(0.0f, 2.5f, 10.0f);
	spotLight.Direction = vec3(0.0f, .3333f, 1.0f);
	spotLight.CosAngle = cos(12.5 * DEG_TO_RAD);
	//Lo += CalculateSpotLight(spotLight);
	

	vec3 ambient = vec3(0.005f) * outMaterial.Albedo * outMaterial.AmbientOcclusion; // Apply ambient lighting
	vec3 colour = ambient + Lo;
	
	// Gamma tonemapping
	colour = colour / (colour + vec3(1.0f));
	colour = pow(colour, vec3(1.0f / 2.2f));
	
	FragColour = vec4(colour, 1.0f);
	
	//FragColour = vec4(outMaterial.Albedo, 1.0f);
	//FragColour = vec4(outMaterial.Metalness);
	
	//FragColour = vec4(ViewDirection, 1.0f);
	//FragColour = vec4(max(dot(ViewDirection, PixelNormal), 0.0f));
	
	//FragColour = vec4(ViewDirection.xyz, 1.0f);
	
	//FragColour = vec4(ViewDirection.xyz, 1.0f);
	
	
	FragColour = vec4(TonemapSCurve_ACES(FragColour.xyz), 1.0f);
} 