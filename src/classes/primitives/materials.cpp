#include "primitives/materials.h"

void DefaultMaterial::Bind() const
{
	shader->Bind();
}

void UnlitMaterial::Bind() const
{
	shader->Bind();
	shader->SetUniform3fv("color", color);
}

void TextureMaterial::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	shader->Bind();
	shader->SetUniform1i("tex0", 0);
}

void PhongTextureMaterial::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTexID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTexID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionTexID);

	shader->Bind();
	shader->SetUniform1i("material.diffuse", 0);
	shader->SetUniform1i("material.specular", 1);
	shader->SetUniform1i("material.emission", 2);
	shader->SetUniform1f("material.shininess", shininess);

	glActiveTexture(GL_TEXTURE0);
}

void PhongMaterial::Bind() const
{
	shader->Bind();
	shader->SetUniform3fv("material.diffuse", diffuse);
	shader->SetUniform3fv("material.specular", specular);
	shader->SetUniform1f("material.shininess", shininess);
}