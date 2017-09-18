#version 120

centroid varying vec2 UV;
centroid varying vec3 color;
centroid varying vec3 viewSpace;

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
	vec4 col = texCol * vec4(color, 1);
	float dist = length(viewSpace);
	float tmp = max(0, dist - fogDistance);
	float fog = clamp(1 / exp(tmp * tmp * 0.01), 0, 1);
	gl_FragColor = mix(col, fogColor, 1 - fog);
}
