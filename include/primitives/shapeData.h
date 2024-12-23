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
	GLushort* indices;
	GLuint numIndices;

	GLsizeiptr GetVertexBufferSize() const { return numVertices * sizeof(Vertex); }
	GLsizeiptr GetIndexBufferCount() const { return numIndices; }

	void CleanUp()
	{
		delete[] vertices;
		delete[] indices;
		numVertices = numIndices = 0;
	}
};

#endif