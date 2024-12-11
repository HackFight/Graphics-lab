#include "primitives/vao.h"

VAO::VAO()
{
	glGenVertexArrays(1, &m_rendererID);
}

VAO::~VAO()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

void VAO::Bind() const
{
	glBindVertexArray(m_rendererID);
}

void VAO::Unbind() const
{
	glBindVertexArray(0);
}


void VAO::AddBuffer(const VBO& vbo, const VBOLayout& vboL, GLuint attribOffset)
{
	Bind();
	vbo.Bind();
	const auto& elements = vboL.GetElements();
	GLuint offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i + attribOffset);
		glVertexAttribPointer(i + attribOffset, element.count, element.type, element.normalized, vboL.GetStride(), (const void*)offset);

		offset += element.count * sizeof(element.type);
	}
}