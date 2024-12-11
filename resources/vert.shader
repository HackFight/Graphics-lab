#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

out vec3 color;

uniform mat4 transformMatrix;

void main()
{
	gl_Position = transformMatrix * vec4(position, 1.0f);
	color = vertexColor;
}