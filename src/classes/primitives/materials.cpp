#include "primitives/materials.h"

void DefaultMaterial::Bind() const
{
	shader->Bind();
}

void PhongEmissionMaterial::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, emissionTex);

	shader->Bind();
	shader->SetUniform1i("colorTexUnit", 0);
	shader->SetUniform1i("specularTexUnit", 1);
	shader->SetUniform1i("emissionTexUnit", 2);

	glActiveTexture(GL_TEXTURE0);
}