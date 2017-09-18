#version 120

centroid varying vec3 color;

void main()
{
	vec4 col = vec4(color, 1);
	gl_FragColor = col;
}
