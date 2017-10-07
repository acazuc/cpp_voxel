#version 120

centroid varying vec2 UV;
centroid varying vec3 viewSpace;

uniform sampler2D tex;
uniform float fogDistance;
uniform vec4 fogColor;
uniform vec4 color;

void main()
{
	vec4 texCol = texture2D(tex, UV);
	vec4 col = texCol * color;
	float dist = length(viewSpace);
	float tmp = max(0, dist - fogDistance);
	float fog = clamp(exp(-tmp * .1), 0, 1);
	gl_FragColor = mix(col, fogColor, 1 - fog);
}
