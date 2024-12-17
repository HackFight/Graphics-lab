#version 430

out vec4 FragColor;

in vec2 UV;

uniform sampler2D tex0;

void main()
{
	FragColor = texture(tex0, UV);
}