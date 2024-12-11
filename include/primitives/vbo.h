#ifndef VBO_CLASS
#define VBO_CLASS
#include <glad/glad.h>

class VBO
{
private:
	GLuint m_rendererID;

public:
	VBO(GLsizeiptr size, const void* data);
	~VBO();

	void Bind() const;
	void Unbind() const;
};

#endif