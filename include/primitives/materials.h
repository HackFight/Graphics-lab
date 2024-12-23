#ifndef MATERIAL_CLASS
#define MATERIAL_CLASS
#include <glm/glm.hpp>
#include <primitives/texture.h>
#include <primitives/shader.h>

class Material
{
public:
	Shader* shader;
	void Bind() const;
};

class UnlitMaterial : public Material
{
public:
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

	void Bind() const;
};

class PhongMaterial : public Material
{
public:
	glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);;
	float shininess = 32.0f;

	void Bind() const;
};

#endif