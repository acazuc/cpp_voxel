#version 330

in vec2 UV;
in vec3 color;
in vec3 viewSpace;

out vec4 output;

uniform sampler2D tex;
uniform float fogDistance;
uniform vec4 fogColor;

void main()
{
	vec4 texCol = texture2D(tex, UV);
	vec4 col = texCol * vec4(color, 1);
	float dist = length(viewSpace);
	float tmp = max(0, dist - fogDistance);
	float fog = clamp(exp(-tmp * .1), 0, 1);
	output = mix(col, fogColor, 1 - fog);
}
