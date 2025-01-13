#ifndef TEXTURE_CLASS
#define TEXTURE_CLASS
#include <glad/glad.h>
#include <stb_image/stb_image.h>

class Texture
{
private:
	GLuint m_rendererID;
	GLenum m_texture;

public:
	const char* m_type;
	GLuint getID() { return m_rendererID; };

	Texture(GLenum texture, const char* type, const char* filepath, GLenum format, bool nearest);

	void Bind() const;
	void Unbind() const;
};

#endif