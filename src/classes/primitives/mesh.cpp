#include "primitives/mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLushort> indices, std::vector<Texture> textures):
	m_vertices(vertices),
	m_indices(indices),
	m_textures(textures)
{
	VBO vbo(vertices);
	VBOLayout vboL;
	vboL.Push<GLfloat>(3);	//Set position attrib
	vboL.Push<GLfloat>(2);	//Set UV attrib
	vboL.Push<GLfloat>(3);	//Set normals attrib
	vboL.Push<GLfloat>(3);	//Set color attrib

	m_vao.AddBuffer(vbo, vboL, 0);

	EBO ebo(indices);

	//Cleanup
	m_vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}

void Mesh::Bind()
{
	m_vao.Bind();
}