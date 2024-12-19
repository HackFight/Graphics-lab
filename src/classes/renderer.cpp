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

void Renderer::Draw(const VAO& vao, const EBO& ebo, Shader& shader) const
{
	vao.Bind();
	shader.Bind();
	shader.SetUniformMatrix4fv("proj", m_projectionMatrix);
	shader.SetUniformMatrix4fv("view", m_viewMatrix);
	glDrawElements(GL_TRIANGLES, ebo.GetIndicesCount(), GL_UNSIGNED_SHORT, 0);
}

void Renderer::DrawMaterial(const VAO& vao, const EBO& ebo, const DefaultMaterial& material) const
{
	vao.Bind();
	material.Bind();
	material.shader->SetUniformMatrix4fv("proj", m_projectionMatrix);
	material.shader->SetUniformMatrix4fv("view", m_viewMatrix);
	glDrawElements(GL_TRIANGLES, ebo.GetIndicesCount(), GL_UNSIGNED_SHORT, 0);
}