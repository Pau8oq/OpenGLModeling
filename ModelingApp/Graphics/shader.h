#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexShaderPath, const char* fragmentShaderPath);

	void active();
	void clear();
	void setInt(const std::string& name, int value);
	void set3Float(const std::string& name, glm::vec3 value);
	void set3Float(const std::string& name, float v1, float v2, float v3);
	void setMat4(const std::string& name, glm::mat4 value);

private:
	void generate(const char* vertexShaderPath, const char* fragmentShaderPath);
	GLuint compileShader(const char* shaderPath, GLenum type);
	std::string loadShader(const char* path);
};