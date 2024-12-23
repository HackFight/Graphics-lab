#include "primitives/vbo.h"

VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &m_rendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

}

VBO::~VBO()
{
	glDeleteBuffers(1, &m_rendererID);
}

void VBO::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_rendererID);
}

void VBO::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}