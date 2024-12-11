#version 430

out vec4 finalColor;

in vec3 fColor;

void main()
{
	finalColor = vec4(fColor, 1.0);
}