#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexColor;
attribute vec2 vertexUV;

centroid varying vec2 UV;
centroid varying vec3 color;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vertexPosition, 1);
	UV = vertexUV;
	color = vertexColor;
}
