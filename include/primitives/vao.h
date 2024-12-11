#ifndef VAO_CLASS
#define VAO_CLASS
#include <primitives/vbo.h>
#include <primitives/vboLayout.h>

class VAO
{
private:
	GLuint m_rendererID;

public:
	VAO();
	~VAO();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VBO& vbo, const VBOLayout& vboL, GLuint attribOffset);
};

#endif