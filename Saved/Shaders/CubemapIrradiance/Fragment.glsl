#version 330 core
out vec4 FragColour; 

in vec3 LocalPosition;






uniform vec3 MinBounds;
uniform vec3 MaxBounds;
vec3 Size = MaxBounds - MinBounds;
uniform vec3 CameraPosition;
uniform vec3 CameraDirection;
uniform float ElapsedTime;



#define PI 3.14159265359
#define INV_PI 0.31830988618
#define DEG_TO_RAD 0.0174533
#define RAD_TO_DEG 57.2958

#define saturate(x) clamp(x, 0.0f, 1.0f)
#define sqr(x)



vec3 GetLocalUVW(vec3 LocalPosition)
{
	return (LocalPosition - MinBounds) / Size;
}



const vec2 INV_ATAN = vec2(0.1591, 0.3183);

vec2 SphericalUVsFromPosition(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= INV_ATAN;
	uv += 0.5f;
	return uv;
}

vec4 SampleCubemap(sampler2D TexCube, vec3 Position)
{
	vec2 uv = SphericalUVsFromPosition(Position);
	return texture(TexCube, uv.xy);
}



/* Transforms vector by matrix */
vec3 TransformVector(vec3 InVector, mat4 Matrix)
{
	return mul(vec4(InVector, 0.0f), Matrix).xyz;
}
/* Transforms position by matrix */
vec3 TransformPosition(vec3 InPosition, mat4 Matrix)
{
	return mul(vec4(InPosition, 1.0f), Matrix).xyz;
}



uniform samplerCube MapToProcess;




void main()
{
	vec3 envColour = vec3(1.0f, 0.0f, 0.0f);
	envColour = texture(MapToProcess, LocalPosition.xyz).rgb;
	
	
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
			
			
			vec3 tangentSample = vec3(SIN_THETA * COS_PHI, SIN_THETA * SIN_PHI, COS_THETA);
			
			vec3 sample = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N;
			
			irradiance += texture(MapToProcess, sample).rgb * COS_THETA * SIN_THETA;
			++accumulatedSamples;
		}
	}
	irradiance = PI * irradiance * (1.0f / float(accumulatedSamples));
	
	
	
	
	FragColour = vec4(irradiance, 1.0f);
}
