#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void active();
	void clear();
	void setInt(const std::string& name, int value);

private:
	void generate(const char* vertexShaderPath, const char* fragmentShaderPath);
	GLuint compileShader(const char* shaderPath, GLenum type);
	std::string loadShader(const char* path);
};