#version 330

in vec3 color;
in vec4 viewSpace;

out vec4 output;

uniform float fogDistance;
uniform vec4 fogColor;

void main()
{
	vec4 col = vec4(color, .8);
	float dist = length(viewSpace);
	float tmp = max(0, dist - fogDistance);
	float fog = clamp(exp(-tmp * .1), 0, 1);
	output = mix(col, fogColor, 1 - fog);
}
