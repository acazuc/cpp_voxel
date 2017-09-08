#version 120

centroid varying vec3 color;
centroid varying vec4 viewSpace;

uniform float fogDistance;
uniform vec4 fogColor;

void main()
{
	vec4 col = vec4(color, 1);
	float dist = length(viewSpace);
	float tmp = max(0, dist - fogDistance);
	float fog = clamp(1 / exp(pow(tmp, 2) * 0.01), 0, 1);
	gl_FragColor = mix(col, fogColor, 1 - fog);
}
