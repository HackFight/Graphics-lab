#ifndef SHAPEGENERATOR_CLASS
#define SHAPEGENERATOR_CLASS
#include <glm/glm.hpp>
#include <primitives\shapeData.h>
#include <primitives\vertex.h>

class ShapeGenerator
{
public:
	static ShapeData MakeTriangle();
	static ShapeData MakeSquare();
	static ShapeData MakeCube();
};

#endif