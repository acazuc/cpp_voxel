#version 330

in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 color;
out vec4 viewSpace;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;

void main()
{
	vec4 newVertex = vec4(vertexPosition, 1);
	color = vertexColor;
	viewSpace = V * M * newVertex;
	gl_Position = MVP * newVertex;
}
