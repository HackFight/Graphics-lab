#ifndef SHAPEDATA
#define SHAPEDATA
#include <openglErrorReporting.h>
#include <primitives/vertex.h>

struct ShapeData
{
	ShapeData() :
		vertices(NULL), numVertices(0),
		indices(NULL), numIndices(0) {}

	Vertex* vertices;
	GLuint numVertices;
	GLuint* indices;
	GLuint numIndices;

	GLsizeiptr GetVertexBufferSize() const { return numVertices * sizeof(Vertex); }
	GLsizeiptr GetVertexBufferCount() const { return numVertices; }
	GLsizeiptr GetIndexBufferCount() const { return numIndices; }

	void CleanUp()
	{
		delete[] vertices;
		delete[] indices;
		numVertices = numIndices = 0;
	}
};

#endif