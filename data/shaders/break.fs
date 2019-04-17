#version 330

in vec2 UV;
in vec3 viewSpace;

out vec4 output;

uniform sampler2D tex;
uniform float fogDistance;
uniform vec4 fogColor;
uniform int disableTex;
uniform float timeFactor;

void main()
{
	vec4 texCol = texture2D(tex, UV);
	if (texCol.r == 127. / 255.)
		discard;
	float dist = length(viewSpace);
	float tmp = max(0, dist - fogDistance);
	float fog = clamp(exp(-tmp * .1), 0, 1);
	output = mix(texCol, fogColor, 1 - fog);
}
