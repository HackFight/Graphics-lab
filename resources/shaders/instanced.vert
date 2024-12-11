#version 430

in layout(location=0) vec3 aPosition;
in layout(location=1) vec3 aColor;
in layout(location=2) vec2 aOffset;

out vec3 fColor;

void main()
{
	gl_Position = vec4(aPosition.x + aOffset.x, aPosition.y + aOffset.y, aPosition.z, 1.0f);
	fColor = aColor;
}