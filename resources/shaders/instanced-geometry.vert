#version 430

in layout(location=0) vec3 aPos;
in layout(location=1) vec3 aNorm;
in layout(location=2) vec3 aColor;
in layout(location=3) mat4 aModel;

uniform mat4 proj;
uniform mat4 view;

out vec3 Normal;
out vec3 Color;
out vec3 FragPos;

void main()
{
	gl_Position = proj * view * aModel * vec4(aPos, 1.0f);
	
	Normal = aNorm;
	Color = aColor;
	FragPos = vec3(aModel * vec4(aPos, 1.0));
}