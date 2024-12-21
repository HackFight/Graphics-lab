#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS
#include <glad/glad.h>
#include <stb/stb_img.h>

class Texture
{
private:
	GLuint m_rendererID;
	GLenum m_texture;

public:
	Texture(GLenum texture, const char* filepath, GLenum format, bool nearest);
	~Texture();

	void Bind() const;
	void Unbind() const;

	GLuint GetID() const
	{
		return m_rendererID;
	}
};

#endif