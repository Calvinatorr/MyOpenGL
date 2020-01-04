#version 330 core

vec3 PixelNormal = normalize(VertexNormal); // Not always correctly normalized from vertex->fragment interpolation
vec3 ViewDirection = normalize(CameraPosition - WorldPosition);