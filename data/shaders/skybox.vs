#version 330

in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 color;

uniform mat4 MVP;

void main()
{
	color = vertexColor;
	gl_Position = MVP * vec4(vertexPosition, 1);
}
