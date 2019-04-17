#version 330

in vec2 UV;
in vec4 color;

out vec4 output;

uniform sampler2D tex;

void main()
{
	vec4 texCol = texture2D(tex, UV);
	vec4 col = color * texCol;
	if (col.a == 0)
		discard;
	output = col;
}
