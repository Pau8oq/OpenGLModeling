#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

class Texture
{
public:
	unsigned int id;
	std::string name;
	std::string path;

	Texture();
	Texture(std::string path, std::string name);

	void active_bind();

private:
	void generate();
	void load();
};