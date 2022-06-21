#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

class Screen
{
public:
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGTH;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	Screen();
	Screen(std::string name);

	bool init();
	void setParameters();

	void update();
	void newFrame();

	bool shouldClose();
	void setShouldClose(bool shouldClose);

private:
	std::string name;

	GLFWwindow* window;
};