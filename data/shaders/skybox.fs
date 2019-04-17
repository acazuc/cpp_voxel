#version 330

in vec3 color;

out vec4 output;

void main()
{
	vec4 col = vec4(color, 1);
	output = col;
}
