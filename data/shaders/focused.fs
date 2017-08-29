#version 120

centroid varying vec4 color;
centroid varying vec4 viewSpace;

uniform float fogDistance;
uniform vec4 fogColor;

void main()
{
	float dist = length(viewSpace);
	float fog = clamp(1 / exp(pow(max(0, dist - fogDistance), 2) * 0.01), 0, 1);
	gl_FragColor = mix(color, fogColor, 1 - fog);
}
