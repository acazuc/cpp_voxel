#version 330

in vec3 vertexPosition;
in vec3 vertexColor;
in vec2 vertexUV;

out vec2 UV;
out vec3 color;
out vec3 viewSpace;

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
