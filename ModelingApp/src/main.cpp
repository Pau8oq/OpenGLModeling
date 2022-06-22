#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../IO/screen.h"


void process_input(GLFWwindow* window);

Screen screen;

float vertices[] = {
	 0.5f,  0.5f, 0.0f,  
	 0.5f, -0.5f, 0.0f, 
	-0.5f, -0.5f, 0.0f,  
	-0.5f,  0.5f, 0.0f 
};

unsigned int indices[] =
{
	0, 1, 3,
	1, 2, 3
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif 

	if (!screen.init())
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	screen.setParameters();
	
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

#pragma region Shaders
	//shader
	std::ifstream file;
	std::stringstream buf;
	std::string ret;

	file.open("assets/shaders/vertex.shader");
	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
		buf.str("");//clear
	}
	else
	{
		std::cout << "Could not open vertex.shader file" << std::endl;
	}
	file.close();

	const GLchar* shaderSrc = ret.c_str();

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &shaderSrc, NULL);
	glCompileShader(vertexShader);


	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	file.open("assets/shaders/fragment.shader");
	if (file.is_open())
	{
		buf << file.rdbuf();
		ret = buf.str();
		buf.str("");//clear
	}
	else
	{
		std::cout << "Could not open fragment.shader file" << std::endl;
	}
	file.close();

	const GLchar* fragShaderSrc = ret.c_str();
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragmentShader);

	int successF;
	char infoLogF[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &successF);
	if (!successF)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogF);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogF << std::endl;
	}

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int successP;
	char infoLogP[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successP);
	if (!successP) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogP);
		std::cout << "Cannot link shaderProgram\n" << infoLogF << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
#pragma endregion



	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	while (!screen.shouldClose())
	{
		screen.update();

		//process_input(window);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

		screen.newFrame();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}