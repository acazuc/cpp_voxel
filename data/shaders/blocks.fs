#version 120

centroid varying vec2 UV;
centroid varying vec3 color;
centroid varying vec3 viewSpace;

uniform sampler2D tex;
uniform float fogDistance;
uniform float fogDensity;
uniform vec4 fogColor;
uniform int disableTex;
uniform float timeFactor;

void main()
{
	vec4 texCol = vec4(1, 1, 1, 1);
	if (disableTex == 0)
		texCol = texture2D(tex, UV);
	else
	{
		//float rand = fract(sin(dot(viewSpace.xy, vec2(12.9898, 78.233 * timeFactor))) * 43758.5453);
		//texCol = vec4(rand, rand, rand, 1);
		//texCol *= texture2D(tex, UV);
	}
	if (texCol.a == 0)
		discard;
	vec4 col = texCol * vec4(color, 1);
	float dist = length(viewSpace);
	float tmp = max(0, dist - fogDistance);
	float fog = clamp(exp(-tmp * fogDensity), 0, 1);
	gl_FragColor = mix(col, fogColor, 1 - fog);
}
