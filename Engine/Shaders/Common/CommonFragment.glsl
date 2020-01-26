#version 330 core

// Not always correctly normalized from vertex->fragment interpolation
vec3 PixelNormal = normalize(VertexNormal); 
/*#ifdef VertexTangent
vec3 PixelTangent = normalize(VertexTangent);
vec3 PixelBitangent = normalize(cross(PixelNormal, PixelTangent));
#endif*/

vec3 ViewDirection = normalize(CameraPosition - WorldPosition);


/* Smooth step using inverse lerp. Only runs in pixel shader due to using fwidth() (ddx() & ddy()) */ 
float StepAA(float In, float Gradient)
{
	float halfChange = fwidth(Gradient) / 2;
	//base the range of the inverse lerp on the change over one pixel
	float lowerEdge = In - halfChange;
	float upperEdge = In + halfChange;
	//do the inverse interpolation
	float stepped = (Gradient - lowerEdge) / (upperEdge - lowerEdge);
	stepped = saturate(stepped);
	return stepped;
}