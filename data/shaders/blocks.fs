#version 120

centroid varying vec2 UV;
centroid varying vec3 color;
centroid varying vec4 viewSpace;

uniform sampler2D tex;
uniform float fogDistance;
uniform vec4 fogColor;

void main()
{
	vec4 texCol = texture2D(tex, UV);
	vec4 col = texCol * vec4(color, 1);
	float dist = length(viewSpace);
	float fog = clamp(1 / exp(pow(max(0, dist - fogDistance), 2) * 0.01), 0, 1);
	gl_FragColor = mix(col, fogColor, 1 - fog);
}
