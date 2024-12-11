#version 430

in layout(location=0) vec3 aPosition;
in layout(location=1) vec3 aColor;
in layout(location=2) mat4 aModel;

uniform mat4 worldToClip;

out vec3 fColor;

void main()
{
	gl_Position = worldToClip * aModel * vec4(aPosition, 1.0f);
	fColor = aColor;
}