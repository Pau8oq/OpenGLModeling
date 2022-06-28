#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse
{
public:
	static float sensitivity;

	static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);

	
	static double getMouseX();
	static double getMouseY();
	static double getMouseDx();
	static double getMouseDy();
	static double getScrollDx();
	static double getScrollDy();

	static bool button(int button);
	static bool buttonWentUp(int button);
	static bool buttonWentDown(int button);
	static bool buttonChanged(int button);

private:
	static double x;
	static double y;

	static double dx;
	static double dy;

	static double lastX;
	static double lastY;

	static double scrollDx;
	static double scrollDy;

	static bool activeButtons[];
	static bool changedButtons[];
	static bool firstMouse;
};