#ifndef VERTEX
#define VERTEX
#include <glm/glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	glm::vec3 color;
};

#endif