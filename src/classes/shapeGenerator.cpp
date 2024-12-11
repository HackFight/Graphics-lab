#include "shapeGenerator.h"
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

ShapeData ShapeGenerator::MakeTriangle()
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3( 0.0f,  1.0f, 0.0f),   glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),   glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3( 1.0f, -1.0f, 0.0f),   glm::vec3(0.0f, 0.0f, 1.0f),
	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLushort indices[] = { 0, 1, 2 };
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	return ret;
}

ShapeData ShapeGenerator::MakeSquare()
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3(-1.0f,  -1.0f, 0.0f),   glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3( 1.0f,  -1.0f, 0.0f),   glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3( 1.0f,   1.0f, 0.0f),   glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f,   1.0f, 0.0f),   glm::vec3(1.0f, 1.0f, 1.0f),
	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLushort indices[] =
	{ 
		0, 1, 2,
		0, 2, 3
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	return ret;
}

ShapeData ShapeGenerator::MakeCube()
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3( 1.0f, -1.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3( 1.0f,  1.0f,  1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(1.0f, 1.0f, 0.0f),

		glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3( 1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3( 1.0f,  1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f),
	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLushort indices[] =
	{
		0, 1, 2,		//Front face
		0, 2, 3,

		5, 4, 7,		//Back face
		5, 7, 6,

		0, 5, 4,		//Bottom face
		0, 1, 5,

		3, 6, 7,		//Top face
		3, 2, 6,

		4, 0, 3,		//Left face
		4, 3, 7,

		1, 5, 6,		//Right face
		1, 6, 2,
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	return ret;
}