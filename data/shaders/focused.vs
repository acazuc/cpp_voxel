#version 120

attribute vec3 vertexPosition;
attribute vec4 vertexColor;

centroid varying vec4 color;

uniform mat4 MVP;

void main()
{
	vec4 newPosition = vec4(vertexPosition, 1);
	color = vertexColor;
	gl_Position = MVP * newPosition;
}
