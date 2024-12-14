#version 430

in layout(location=0) vec3 aPos;
in layout(location=1) vec3 aNorm;
in layout(location=2) vec3 aColor;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 Normal;
out vec3 Color;
out vec3 FragPos;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1.0);

	Normal = mat3(transpose(inverse(model))) * aNorm;
	Color = aColor;
	FragPos = vec3(model * vec4(aPos, 1.0));
}