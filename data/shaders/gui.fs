#version 120

centroid varying vec2 UV;
centroid varying vec4 color;

uniform sampler2D tex;

void main()
{
	vec4 texCol = texture2D(tex, UV);
	gl_FragColor = color * texCol;
}
