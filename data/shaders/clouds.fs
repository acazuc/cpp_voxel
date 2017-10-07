#version 120

centroid varying vec3 color;
centroid varying vec4 viewSpace;

uniform float fogDistance;
uniform vec4 fogColor;

void main()
{
	vec4 col = vec4(color, .8);
	float dist = length(viewSpace);
	float tmp = max(0, dist - fogDistance);
	float fog = clamp(exp(-tmp * .1), 0, 1);
	gl_FragColor = mix(col, fogColor, 1 - fog);
}
