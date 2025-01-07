#include "renderer.h"

void Renderer::Update(int winWidth, int winHeight, Camera camera)
{
	m_projectionMatrix = glm::perspective(glm::radians(70.0f), (float)winWidth / (float)winHeight, 0.1f, 100.0f);
	m_viewMatrix = camera.GetWorldToViewMatrix();
}

void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::Draw(const VAO& vao, GLuint indicesCount, Shader& shader) const
{
	vao.Bind();
	shader.Bind();
	shader.SetUniformMatrix4fv("proj", m_projectionMatrix);
	shader.SetUniformMatrix4fv("view", m_viewMatrix);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, 0);
}

void Renderer::DrawMaterial(const VAO& vao, GLuint indicesCount, const Material& material) const
{
	vao.Bind();
	material.Bind();
	material.shader->SetUniformMatrix4fv("proj", m_projectionMatrix);
	material.shader->SetUniformMatrix4fv("view", m_viewMatrix);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_SHORT, 0);
}

void Renderer::DrawMesh(const Mesh& mesh, const Material& material) const
{
	unsigned int numDiffuse = 0, numSpecular = 0, numEmission = 0;

	material.Bind();
	mesh.m_vao.Bind();

	for (unsigned int i = 0; i < mesh.m_textures.size(); i++)
	{
		std::string num;
		std::string type = mesh.m_textures[i].m_type;
		if (type == "diffuse")
		{
			num = std::to_string(numDiffuse++);
		}
		else if (type == "specular")
		{
			num = std::to_string(numSpecular++);
		}
		else if (type == "emission")
		{
			num = std::to_string(numEmission++);
		}
		else
		{
			std::cout << "[WARNING]: Texture type \"" << type <<"\" doesn't exist!" << std::endl;
		}

		mesh.m_textures[i].Bind();
		material.shader->SetUniform1i((type + num).c_str(), i);
		
	}

	material.shader->SetUniformMatrix4fv("proj", m_projectionMatrix);
	material.shader->SetUniformMatrix4fv("view", m_viewMatrix);

	glDrawElements(GL_TRIANGLES, mesh.m_indices.size(), GL_UNSIGNED_SHORT, 0);
}
