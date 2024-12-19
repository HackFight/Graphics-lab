#ifndef RENDERER_CLASS
#define RENDERER_CLASS
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <primitives/vao.h>
#include <primitives/ebo.h>
#include <primitives/shader.h>
#include <primitives/materials.h>
#include <camera.h>

class Renderer
{
private:
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
	glm::mat4 m_viewMatrix = glm::mat4(1.0f);

public:
	void Update(int winWidth, int winHeight, Camera camera);
	void Clear() const;
	void Draw(const VAO& vao, const EBO& ebo, Shader& shader) const;
	void DrawMaterial(const VAO& vao, const EBO& ebo, const DefaultMaterial& material) const;
};

#endif