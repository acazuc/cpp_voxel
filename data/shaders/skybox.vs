#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexColor;

centroid varying vec3 color;

uniform mat4 MVP;

void main()
{
	color = vertexColor;
	gl_Position = MVP * vec4(vertexPosition, 1);
}
