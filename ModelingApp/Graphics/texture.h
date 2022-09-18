#pragma once

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <assimp/material.h>

class Texture
{
public:
	unsigned int id;
	std::string name;
	std::string path;
	aiTextureType type;

	Texture();
	Texture(std::string path, std::string name);
	Texture(std::string path, std::string name, aiTextureType type);

	void active_bind();
	void load();

private:
	void generate();
};