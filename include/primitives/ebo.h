#ifndef EBO_CLASS
#define EBO_CLASS
#include <vector>
#include <glad/glad.h>

class EBO
{
private:
	GLuint m_rendererID;
	GLuint m_indicesCount;

public:
	EBO(std::vector<GLushort>& indices);
	~EBO();

	void Bind() const;
	void Unbind() const;

	GLuint GetIndicesCount() const { return m_indicesCount; }
};

#endif