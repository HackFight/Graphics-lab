#include "primitives/texture.h"

Texture::Texture(GLenum texture, const char* type, const char* filepath, GLenum format, bool nearest):
	m_texture(texture),
	m_type(type)
{
	glGenTextures(1, &m_rendererID);
	glActiveTexture(m_texture);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (nearest)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	stbi_set_flip_vertically_on_load(true);
	int imgWidth, imgHeight, numColCh;
	unsigned char* imgBytes = stbi_load(filepath, &imgWidth, &imgHeight, &numColCh, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, imgBytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgBytes);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const
{
	glActiveTexture(m_texture);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}