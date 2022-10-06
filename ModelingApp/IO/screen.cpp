#include "screen.h"
#include "mouse.h"
#include "keyboard.h"

unsigned int Screen::SCR_WIDTH = 800;
unsigned int Screen::SCR_HEIGTH = 600;

void Screen::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	SCR_WIDTH = width;
	SCR_HEIGTH = height;
}

Screen::Screen()
	:name("AppModeling"), window(nullptr)
{}

Screen::Screen(std::string name)
	:name(name), window(nullptr)
{}

bool Screen::init()
{
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGTH, name.c_str(), NULL, NULL);

	if (!window)
		return false;

	glfwMakeContextCurrent(window);
	return true;
}

void Screen::setParameters()
{
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGTH);
	glfwSetWindowSizeCallback(window, Screen::framebuffer_size_callback);

	glfwSetCursorPosCallback(window, Mouse::cursorPosCallback);
	glfwSetMouseButtonCallback(window, Mouse::mouseButtonCallback);
	glfwSetScrollCallback(window, Mouse::mouseWheelCallback);

	glfwSetKeyCallback(window, Keyboard::keyCallback);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void Screen::update()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Screen::newFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool Screen::shouldClose() 
{
	return glfwWindowShouldClose(window);
}

void Screen::setShouldClose(bool shouldClose)
{
	glfwSetWindowShouldClose(window, shouldClose);
}

