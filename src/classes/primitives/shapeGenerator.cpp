#include "primitives/shapeGenerator.h"
#define NUM_ARRAY_ELEMENTS(a) sizeof(a) / sizeof(*a)

ShapeData ShapeGenerator::MakeTriangle()
{
	ShapeData ret;

	Vertex verts[] =
	{
		glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec2(0.5f,  0.5f), glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec2(1.0f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
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
		glm::vec3(-1.0f,  -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3( 1.0f,  -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3( 1.0f,   1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-1.0f,   1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),	glm::vec3(1.0f, 1.0f, 1.0f),
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
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),	//Front face
		glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),	//Back face
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),	//Left face
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),	//Right face
		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),

		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),	//Bottom face
		glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 1.0f),

		glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f),	//Top face
		glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 1.0f)
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

#pragma warning(disable:6385)
#pragma warning(disable:6386)
ShapeData ShapeGenerator::MakePlane(unsigned int divisions, float width)
{
	ShapeData ret;
	
	ret.numVertices = (divisions + 1) * (divisions + 1);
	float triSide = width / divisions;
	float half = (divisions + 1) / 2;
	ret.vertices = new Vertex[ret.numVertices];
	for (unsigned int y = 0; y < (divisions + 1); y++)
	{
		for (unsigned int x = 0; x < (divisions + 1); x++)
		{
			Vertex& vert = ret.vertices[y * (divisions + 1) + x];
			glm::vec3 pos = glm::vec3(x * triSide - width / 2.0f, 0.0f, y * -triSide + width / 2.0f);
			vert.position = pos;
			vert.uv = glm::vec2((float)x / (float)(divisions + 1), (float)y / (float)(divisions + 1));
			vert.normal = glm::vec3(0.0f, 1.0f, 0.0f);
			vert.color = glm::vec3((float)x / (float)(divisions + 1), 1.0f, (float)y / (float)(divisions + 1));
		}
	}

	ret.numIndices = (divisions * divisions) * 2 * 3;
	ret.indices = new unsigned short[ret.numIndices];
	int runner = 0;
	for (unsigned int y = 0; y < divisions; y++)
	{
		for (unsigned int x = 0; x < divisions; x++)
		{
			int index = y * (divisions + 1) + x;					// _____
			ret.indices[runner++] = index;							// |  /
			ret.indices[runner++] = index + (divisions + 1) + 1;	// | /
			ret.indices[runner++] = index + (divisions + 1);		// |/

			ret.indices[runner++] = index;							//   /|
			ret.indices[runner++] = index + 1;						//  / |
			ret.indices[runner++] = index + (divisions + 1) + 1;	// /__|
		}
	}
	assert(runner == ret.numIndices);
	return ret;
}