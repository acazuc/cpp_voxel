#version 120

attribute vec3 vertexPosition;
attribute vec3 vertexColor;
attribute vec2 vertexUV;

centroid varying vec2 UV;
centroid varying vec3 color;
centroid varying vec3 viewSpace;

uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform float timeFactor;

void main()
{
	vec4 newVertex = vec4(vertexPosition, 1);
	/*if (blockId == 18)
	{
		newVertex.x += cos(newVertex.x + newVertex.y + newVertex.z + timeFactor * 3.14) * .1;
		newVertex.y += cos(newVertex.x + newVertex.y + newVertex.z + timeFactor * 3.14 / 3) * .033;
		newVertex.z += cos(newVertex.x + newVertex.y + newVertex.z + timeFactor * 3.14 / 2) * .05;
	}*/
	UV = vertexUV;
	color = vertexColor;
	viewSpace = (V * M * newVertex).xyz;
	gl_Position = MVP * newVertex;
}
