#ifndef SHADER_CLASS
#define SHADER_CLASS
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct ShaderSource
{
	GLuint vertexShaderID;
	GLuint fragmentShaderID;
};

class Shader
{
private:
	GLuint m_rendererID;

	GLuint m_vertexShaderID;
	GLuint m_fragmentShaderID;

	std::string m_vertexFilepath;
	std::string m_fragmentFilepath;

	std::unordered_map<std::string, GLint> m_uniformLocationCache;

public:
	Shader(const char* vertexFilepath, const char* fragmentFilepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, float v0, float v1);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	void SetUniform2fv(const std::string& name, glm::vec2 value);
	void SetUniform3fv(const std::string& name, glm::vec3 value);
	void SetUniform4fv(const std::string& name, glm::vec4 value);

	void SetUniformMatrix4fv(const std::string& name, glm::mat4 value);

private:
	void MakeShaderSource(const char* vertexFilepath, const char* fragmentFilepath);
	GLuint CompileShader();
	void LinkShader();

	GLint GetUniformLocation(const std::string& name);
	std::string get_file_contents(const char* filename) const;
};

#endif