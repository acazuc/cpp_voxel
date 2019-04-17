#version 330

in vec2 UV;
in vec4 color;

out vec4 output;

uniform sampler2D tex;

void main()
{
	vec4 texCol = vec4(1, 1, 1, texture2D(tex, UV).a);
	output = color * texCol;
}
