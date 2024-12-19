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

class PhongEmissionMaterial : public DefaultMaterial
{
public:
	GLuint colorTex;
	GLuint specularTex;
	GLuint emissionTex;

	void Bind() const;
};

#endif