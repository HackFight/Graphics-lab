#ifndef MATERIAL_CLASS
#define MATERIAL_CLASS
#include <glm/glm.hpp>
#include <primitives/shader.h>

class DefaultMaterial
{
public:
	Shader* shader;
	void Bind() const;
};

class UnlitMaterial : public DefaultMaterial
{
public:
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	void Bind() const;
};

class TextureMaterial : public DefaultMaterial
{
public:
	GLuint textureID;

	void Bind() const;
};

class PhongMaterial : public DefaultMaterial
{
public:
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);;
	float shininess = 32.0f;

	void Bind() const;
};

class PhongTextureMaterial : public DefaultMaterial
{
public:
	GLuint colorTexID;
	GLuint specularTexID;
	GLuint emissionTexID;
	float shininess = 32.0f;

	void Bind() const;
};

#endif