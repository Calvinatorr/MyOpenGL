#version 330 core
out vec4 FragColour; 



in vec2 TexCoord;
in vec4 VertexColour;
in vec3 VertexNormal;

in mat4 LocalToWorld;
in vec3 LocalPosition;
in vec3 WorldPosition;




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


vec3 PixelNormal = normalize(VertexNormal); 
vec3 ViewDirection = normalize(CameraPosition - WorldPosition);


uniform sampler2D tex;
uniform sampler2D tex2;



struct Material
{
	vec3 Albedo;
	float Metalness;
	float Roughness;
	float AmbientOcclusion;
};

uniform Material inMaterial;
Material outMaterial;





float DistributionGGX(float NoH, float Roughness)
{
	float a = Roughness * Roughness;
	float a2 = a * a;
	float NoH2 = NoH * NoH;
	
	float num = a2;
	
	float denom = ( NoH * a2 - NoH ) * NoH + 1;	
	denom = PI * denom * denom;
	
	return num / denom;
}



float GeometrySchlickGGX(float NoV, float Roughness)
{
	float r = Roughness + 1.0f;
	float k = (r * r) / 8.0f;
	
	float num = NoV;
	float denom = NoV * (1.0f - k) + k;
	
	return num / denom;
}

float GeometrySmith(float NoV, float NoL, float Roughness)
{
	float ggx2 = GeometrySchlickGGX(NoV, Roughness);
	float ggx1 = GeometrySchlickGGX(NoL, Roughness);
	
	return ggx1 * ggx2;
}

vec3 FresnelSchlick(float CosTheta, vec3 F0, float Roughness)
{
	return F0 + (1.0f - F0) * pow(1.0f - CosTheta, 5.0f);
}







struct DirectionalLight
{
	vec3 Direction;
	vec3 Radiance;
};

vec3 BlinnPhong(vec3 Direction, vec3 Radiance)
{
	
	vec3 L = normalize(Direction);
	vec3 N = normalize(PixelNormal);
	vec3 V = normalize(ViewDirection);
	
	
	float NoV = max( dot(N, -L), 0.0f );
	vec3 diffuse = vec3(NoV) * Radiance * outMaterial.Albedo;
	
	
	vec3 reflectDir = reflect(-L, N);
	vec3 specular = pow( max(dot(V, reflectDir), 0.0f), 128.0f) * Radiance * 0.5f;
	
	return diffuse + specular;
}

vec3 CalculateRadiance(vec3 N, vec3 V, vec3 L, vec3 Radiance, Material Mat)
{
	
	L = -normalize(L);
	vec3 H = normalize(V + L); 
	float NoV = max(dot(N, V), 0.0f);
	float NoL = max(dot(N, L), 0.0f);
	float NoH = max(dot(N, H), 0.0f);
	float HoV = max(dot(H, V), 0.0f);
	
	vec3 F0 = vec3(0.04f); 
	F0 = mix(F0, Mat.Albedo, Mat.Metalness); 
	
	
	
	float NDF = DistributionGGX(NoH, Mat.Roughness);
	float G = 	GeometrySmith(NoV, NoL, Mat.Roughness);
	vec3  F =	FresnelSchlick(HoV, F0, Mat.Roughness);
	
	
	vec3 kS = F;
	vec3 kD = vec3(1.0f) - kS;
	kD *= 1.0f - Mat.Metalness;
	
	
	vec3 numerator = NDF * G * F;
	float denominator = 4.0f * NoV * NoL;
	vec3 specular = numerator / max(denominator, 0.001f); 
	
	vec3 Lo = ((kD * Mat.Albedo / PI) + specular) * Radiance * NoL;
	return Lo;
}

vec3 CalculateDirectionalLight(DirectionalLight Light)
{
	
	return CalculateRadiance(PixelNormal, ViewDirection, Light.Direction, Light.Radiance, outMaterial);
}

struct PointLight
{
	vec3 Position;
	vec3 Radiance;
};

vec3 CalculatePointLight(PointLight Light)
{
	vec3 dir = WorldPosition - Light.Position;
	float distance = length(dir);
	float attenuation = 1.0f / (distance*distance); 
	
	
	return CalculateRadiance(PixelNormal, ViewDirection, dir, Light.Radiance * attenuation, outMaterial);
}

struct SpotLight
{
	vec3 Position;
	vec3 Direction;
	vec3 Radiance;
	float CosAngle;
};

vec3 CalculateSpotLight(SpotLight Light)
{
	vec3 dir = WorldPosition - Light.Position;
	vec3 L = normalize(-Light.Direction);
	
	float cosTheta = dot(normalize(dir), L);
	if (cosTheta > Light.CosAngle)
	{
		float distance = length(dir);
		float attenuation = 1.0f / (distance*distance); 
		
		return CalculateRadiance(PixelNormal, ViewDirection, dir, Light.Radiance * attenuation, outMaterial);
	}
	else
		return vec3(0.0f);
}





vec3 TonemapSCurve_ACES(vec3 x)
{
  	float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return clamp((x*(a*x+b))/(x*(c*x+d)+e),0.0,1.0);
}




#define NUM_OF_LIGHTS 4
PointLight lights[NUM_OF_LIGHTS];



void main()
{
	
	outMaterial = inMaterial;
	vec4 t = texture(tex, TexCoord.xy);
	vec4 t2 = texture(tex2, TexCoord.xy);
	
	
	
	
		
	
	
	outMaterial.AmbientOcclusion = clamp(outMaterial.AmbientOcclusion, 0.0f, 1.0f);
	outMaterial.Metalness = clamp(outMaterial.Metalness, 0.0f, 1.0f);
	outMaterial.Roughness = clamp(outMaterial.Roughness, 0.0f, 1.0f);
	outMaterial.Albedo = clamp(outMaterial.Albedo, 0.0f, 1.0f);
	
	
	
	
	
	
	
	
	
	vec3 Lo = vec3(0.0f); 
	
	
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
	
	
	PointLight pointLight;
	pointLight.Radiance = vec3(3.142f * 2.0f);
	pointLight.Position = vec3(0.0f, sin(ElapsedTime * 2.0f) * 1.5f, 0.0f);
	
	
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
	
	

	vec3 ambient = vec3(0.005f) * outMaterial.Albedo * outMaterial.AmbientOcclusion; 
	vec3 colour = ambient + Lo;
	
	
	colour = colour / (colour + vec3(1.0f));
	colour = pow(colour, vec3(1.0f / 2.2f));
	
	FragColour = vec4(colour, 1.0f);
	
	
	
	
	
	
	
	
	
	
	
	
	FragColour = vec4(TonemapSCurve_ACES(FragColour.xyz), 1.0f);
} 
