#include "primitives/materials.h"

void Material::Bind() const
{
	shader->Bind();
}

void UnlitMaterial::Bind() const
{
	shader->Bind();
	shader->SetUniform3fv("color", color);
}

void PhongMaterial::Bind() const
{
	shader->Bind();
	shader->SetUniform3fv("material.diffuse", diffuse);
	shader->SetUniform3fv("material.specular", specular);
	shader->SetUniform1f("material.shininess", shininess);
}