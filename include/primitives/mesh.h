#ifndef MESH_CLASS
#define MESH_CLASS
#include <vector>
#include <glad/glad.h>
#include <primitives/vao.h>
#include <primitives/ebo.h>
#include <primitives/vertex.h>
#include <primitives/texture.h>

class Mesh
{
public:
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indices;
	std::vector<Texture> m_textures;

	VAO m_vao;

	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

};

#endif