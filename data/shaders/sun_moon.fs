#version 330

in vec2 UV;
in vec3 color;

out vec4 output;

uniform sampler2D tex;

void main()
{
	vec4 texCol = texture2D(tex, UV);
	vec4 col = texCol * vec4(color, 1);
	output = col;
}
