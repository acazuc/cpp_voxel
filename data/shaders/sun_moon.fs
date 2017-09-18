#version 120

centroid varying vec2 UV;
centroid varying vec3 color;

uniform sampler2D tex;

void main()
{
	vec4 texCol = texture2D(tex, UV);
	vec4 col = texCol * vec4(color, 1);
	gl_FragColor = col;
}
