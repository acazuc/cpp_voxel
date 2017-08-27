#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexColor;

centroid varying vec3 color;
centroid varying vec4 viewSpace;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;

void main()
{
	vec4 newVertex = vec4(vertexPosition, 1);
	gl_Position = MVP * newVertex;
	color = vertexColor;
	viewSpace = V * M * newVertex;
}
