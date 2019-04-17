#version 330

in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexUV;

out vec2 UV;
out vec3 color;

uniform mat4 MVP;

void main()
{
	UV = vertexUV;
	color = vertexColor;
	gl_Position = MVP * vec4(vertexPosition, 1);
}
