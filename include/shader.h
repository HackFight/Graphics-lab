#ifndef SHADER_CLASS
#define SHADER_CLASS
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <glad/glad.h>

class Shader
{
private:
	GLuint m_rendererID;

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

	void SetUniformMatrix4fv(const std::string& name, GLfloat* value);

private:
	GLint GetUniformLocation(const std::string& name);
	std::string get_file_contents(const char* filename);
};

#endif