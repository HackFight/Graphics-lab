#include "primitives/shader.h"

Shader::Shader(const char* vertexFilepath, const char* fragmentFilepath)
	: m_vertexFilepath(vertexFilepath), m_fragmentFilepath(fragmentFilepath), m_rendererID(0)
{
	std::string vertexCode = get_file_contents(vertexFilepath);
	std::string fragmentCode = get_file_contents(fragmentFilepath);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShaderID, 1, &vertexSource, NULL);
	glShaderSource(fragmentShaderID, 1, &fragmentSource, NULL);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	m_rendererID = glCreateProgram();

	glAttachShader(m_rendererID, vertexShaderID);
	glAttachShader(m_rendererID, fragmentShaderID);
	glLinkProgram(m_rendererID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

Shader::~Shader()
{
	glDeleteProgram(m_rendererID);
}

GLint Shader::GetUniformLocation(const std::string& name)
{
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
	{
		return m_uniformLocationCache[name];
	}

	GLint location = glGetUniformLocation(m_rendererID, name.c_str());
	if (location == -1)
	{
		std::cout << "[WARNING]: Uniform" << name << " doesn't exist!" << std::endl;
	}

	m_uniformLocationCache[name] = location;
	return location;
}

std::string Shader::get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

void Shader::Bind() const
{
	glUseProgram(m_rendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform2f(const std::string& name, float v0, float v1)
{
	glUniform2f(GetUniformLocation(name), v0, v1);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2 ,v3);
}

void Shader::SetUniformMatrix4fv(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::SetUniform2fv(const std::string& name, glm::vec2 value)
{
	glUniform2f(GetUniformLocation(name), value.x, value.y);
}

void Shader::SetUniform3fv(const std::string& name, glm::vec3 value)
{
	glUniform3f(GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetUniform4fv(const std::string& name, glm::vec4 value)
{
	glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);
}