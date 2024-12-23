#ifndef VBO_CLASS
#define VBO_CLASS
#include <vector>
#include <glad/glad.h>
#include <primitives/vertex.h>

class VBO
{
private:
	GLuint m_rendererID;

public:
	VBO(std::vector<Vertex>& vertices);
	~VBO();

	void Bind() const;
	void Unbind() const;
};

#endif