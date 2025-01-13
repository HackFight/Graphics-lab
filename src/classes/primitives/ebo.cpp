#include "primitives/ebo.h"

EBO::EBO(std::vector<GLuint>& indices)
	: m_indicesCount(indices.size() * sizeof(GLuint))
{
	glGenBuffers(1, &m_rendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indicesCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

EBO::~EBO()
{
	glDeleteBuffers(1, &m_rendererID);
}

void EBO::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererID);
}

void EBO::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}