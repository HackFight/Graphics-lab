#include "primitives/texture.h"

Texture::Texture(GLenum texture, const char* filepath, GLenum format)
	: m_texture(texture)
{
	glGenTextures(1, &m_rendererID);
	glActiveTexture(m_texture);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int imgWidth, imgHeight, numColCh;
	unsigned char* imgBytes = stbi_load(filepath, &imgWidth, &imgHeight, &numColCh, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, imgBytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(imgBytes);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_rendererID);
}

void Texture::Bind() const
{
	glActiveTexture(m_texture);
	glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void Texture::Unbind() const
{
	glActiveTexture(m_texture);
	glBindTexture(GL_TEXTURE_2D, 0);
}