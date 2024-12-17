#include "primitives/shader.h"

Shader::Shader(const char* vertexFilepath, const char* fragmentFilepath)
	: m_vertexFilepath(vertexFilepath), m_fragmentFilepath(fragmentFilepath), m_rendererID(0)
{
	MakeShaderSource(vertexFilepath, fragmentFilepath);

	m_rendererID = CompileShader();

	LinkShader();
}

Shader::~Shader()
{
	glDeleteProgram(m_rendererID);
}

void Shader::Bind() const
{
	glUseProgram(m_rendererID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::MakeShaderSource(const char* vertexFilepath, const char* fragmentFilepath)
{
	std::string vertexCode = get_file_contents(vertexFilepath);
	std::string fragmentCode = get_file_contents(fragmentFilepath);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(m_vertexShaderID, 1, &vertexSource, NULL);
	glShaderSource(m_fragmentShaderID, 1, &fragmentSource, NULL);
}

GLuint Shader::CompileShader()
{
	glCompileShader(m_vertexShaderID);
	glCompileShader(m_fragmentShaderID);

	return glCreateProgram();
}

void Shader::LinkShader()
{
	glAttachShader(m_rendererID, m_vertexShaderID);
	glAttachShader(m_rendererID, m_fragmentShaderID);
	glLinkProgram(m_rendererID);

	glDeleteShader(m_vertexShaderID);
	glDeleteShader(m_fragmentShaderID);
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

std::string Shader::get_file_contents(const char* filename) const
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


void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}


void Shader::SetUniformMatrix4fv(const std::string& name, glm::mat4 value)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
}