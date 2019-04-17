#version 330

in vec2 vertexPosition;
in vec4 vertexColor;
in vec2 vertexUV;

out vec2 UV;
out vec4 color;

uniform mat4 MVP;

void main()
{
	vec4 newVertex = vec4(vertexPosition, 0, 1);
	UV = vertexUV;
	color = vertexColor;
	gl_Position = MVP * newVertex;
}
