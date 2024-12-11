#ifndef EBO_CLASS
#define EBO_CLASS
#include <glad/glad.h>

class EBO
{
private:
	GLuint m_rendererID;
	GLuint m_indicesCount;

public:
	EBO(GLuint count, const GLushort* data); //size in bytes, count in elements
	~EBO();

	void Bind() const;
	void Unbind() const;

	GLuint GetIndicesCount() const { return m_indicesCount; }
};

#endif