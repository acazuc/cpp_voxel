#version 120

attribute vec3 vertexPosition;
attribute vec4 vertexColor;

centroid varying vec4 color;
centroid varying vec4 viewSpace;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;

void main()
{
	vec4 newPosition = vec4(vertexPosition, 1);
	color = vertexColor;
	viewSpace = V * M * newPosition;
	gl_Position = MVP * newPosition;
}
