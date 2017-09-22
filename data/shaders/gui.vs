#version 120

attribute vec2 vertexPosition;
attribute vec4 vertexColor;
attribute vec2 vertexUV;

centroid varying vec2 UV;
centroid varying vec4 color;

uniform mat4 MVP;

void main()
{
	vec4 newVertex = vec4(vertexPosition, 0, 1);
	UV = vertexUV;
	color = vertexColor;
	gl_Position = MVP * newVertex;
}
