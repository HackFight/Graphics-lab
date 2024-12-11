#ifndef VBOLAYOUT_CLASS
#define VBOLAYOUT_CLASS
#include <vector>
#include <glad/glad.h>

struct LayoutElement
{
	GLint count;
	GLenum type;
	GLboolean normalized;
};

class VBOLayout
{
private:
	std::vector<LayoutElement> m_elements;
	GLuint m_stride;

public:
	VBOLayout();

	template<typename T>
	void Push(GLint count)
	{
		static_assert(false);
	}

	template<>
	void Push<GLfloat>(GLint count)
	{
		m_elements.push_back({ count, GL_FLOAT, GL_FALSE });
		m_stride += count * sizeof(GLfloat);
	}

	template<>
	void Push<GLuint>(GLint count)
	{
		m_elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		m_stride += count * sizeof(GLuint);
	}

	inline const std::vector<LayoutElement> GetElements() const& { return m_elements; }
	inline GLuint GetStride() const { return m_stride; }
};

#endif