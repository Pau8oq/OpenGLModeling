#include "shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	generate(vertexShaderPath, fragmentShaderPath);
}

void Shader::generate(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	int success;
	char infoLog[512];
	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success) 
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Cannot link shaderProgram\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

GLuint Shader::compileShader(const char* shaderPath, GLenum type)
{
	std::string shaderSourceStr = loadShader(shaderPath);
	const GLchar* shaderSourceChar = shaderSourceStr.c_str();

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &shaderSourceChar, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Error with shader::" << shaderPath << "\n" << infoLog << std::endl;
	}

	return shader;
}

std::string Shader::loadShader(const char* path)
{
	std::ifstream file;
	std::stringstream buffer;
	std::string ret;

	file.open(path);

	if (file.is_open())
	{
		buffer << file.rdbuf();
		ret = buffer.str();
	}
	else
	{
		std::cout << "Could not open file path " << path << std::endl;
	}

	file.close();

	return ret;
}

void Shader::active()
{
	glUseProgram(ID);
}

void Shader::clear()
{
	glDeleteProgram(ID);
}

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::set3Float(const std::string& name, glm::vec3 value)
{
	set3Float(name, value.x, value.y, value.z);
}

void Shader::set3Float(const std::string& name, float v1, float v2, float v3)
{
	unsigned int loacation = glGetUniformLocation(ID, name.c_str());

	glUniform3f(loacation, v1, v2, v3);
}

void Shader::setMat4(const std::string& name, glm::mat4 value)
{
	unsigned int loacation = glGetUniformLocation(ID, name.c_str());

	glUniformMatrix4fv(loacation, 1, GL_FALSE, glm::value_ptr(value));
}
