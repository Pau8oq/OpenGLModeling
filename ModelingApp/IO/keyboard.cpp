#include "keyboard.h"

bool Keyboard::activeKeys[GLFW_KEY_LAST] = { 0 };
bool Keyboard::changedKeys[GLFW_KEY_LAST] = { 0 };

void Keyboard::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		activeKeys[key] = true;
	}
	else
		activeKeys[key] = false;

	changedKeys[key] = action != GLFW_REPEAT;
}

bool Keyboard::key(int key)
{
	return activeKeys[key];
}

bool Keyboard::keyWentUp(int key)
{
	return !activeKeys[key] && keyChanged(key);
}

bool Keyboard::keyWentDown(int key)
{
	return activeKeys[key] && keyChanged(key);
}

bool Keyboard::keyChanged(int key)
{
	bool ret = changedKeys[key];
	changedKeys[key] = false;

	return ret;
}