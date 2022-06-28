#include "mouse.h"

double Mouse::x = 0;
double Mouse::y = 0;

double Mouse::dx = 0;
double Mouse::dy = 0;

double Mouse::lastX = 0;
double Mouse::lastY = 0;

double Mouse::scrollDx = 0;
double Mouse::scrollDy = 0;

float Mouse::sensitivity = 0.1f;
bool Mouse::firstMouse = true;

bool Mouse::activeButtons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Mouse::changedButtons[GLFW_MOUSE_BUTTON_LAST] = { 0 };

void Mouse::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	x = xpos;
	y = ypos;

	dx = x - lastX;
	dy = lastY - y; //inverted

	dx *= sensitivity;
	dy *= sensitivity;

	lastX = x;
	lastY = y;
}

void Mouse::mouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	scrollDx = xoffset;
	scrollDy = yoffset;
}

void Mouse::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		activeButtons[button] = true;
	}
	else
		activeButtons[button] = false;

	changedButtons[button] = action != GLFW_REPEAT;

}


bool Mouse::button(int button)
{
	return activeButtons[button];
}

double Mouse::getMouseX()
{
	return x;
}

double Mouse::getMouseY()
{
	return y;
}

double Mouse::getMouseDx()
{
	double _dx = dx;
	dx = 0;
	return _dx;
}

double Mouse::getMouseDy()
{
	double _dy = dy;
	dy = 0;
	return _dy;
}

double Mouse::getScrollDx()
{
	double dx = scrollDx;
	scrollDx = 0;
	return dx;
}

double Mouse::getScrollDy()
{
	double dy = scrollDy;
	scrollDy = 0;
	return dy;
}

bool Mouse::buttonChanged(int button)
{
	bool ret = changedButtons[button];
	changedButtons[button] = false;

	return ret;
}

bool Mouse::buttonWentDown(int button)
{
	return activeButtons[button] && buttonChanged(button);
}


bool Mouse::buttonWentUp(int button)
{
	return !activeButtons[button] && buttonChanged(button);
}