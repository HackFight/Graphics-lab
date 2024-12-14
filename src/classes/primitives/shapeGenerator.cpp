#include "primitives/shapeGenerator.h"
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

ShapeData ShapeGenerator::MakeTriangle()
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3( 0.0f,  1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3( 1.0f, -1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
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
		glm::vec3(-1.0f,  -1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3( 1.0f,  -1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3( 1.0f,   1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f,   1.0f, 0.0f),	glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(1.0f, 1.0f, 1.0f),
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
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),	//Front face
		glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),	//Back face
		glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),	//Left face
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),	//Right face
		glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),

		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),	//Bottom face
		glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),

		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f),	//Top face
		glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f)
	};
	ret.numVertices = NUM_ARRAY_ELEMENTS(verts);
	ret.vertices = new Vertex[ret.numVertices];
	memcpy(ret.vertices, verts, sizeof(verts));

	GLushort indices[] =
	{
		 0,  1,  2,		//Front face
		 0,  2,  3,

		 4,  5,  6,		//Back face
		 4,  6,  7,

		 8,  9, 10,		//Left face
		 8, 10, 11,

		12, 13, 14,		//Right face
		12, 14, 15,

		16, 17, 18,		//Botom face
		16, 18, 19,

		20, 21, 22,		//Top face
		20, 22, 23
	};
	ret.numIndices = NUM_ARRAY_ELEMENTS(indices);
	ret.indices = new GLushort[ret.numIndices];
	memcpy(ret.indices, indices, sizeof(indices));

	return ret;
}