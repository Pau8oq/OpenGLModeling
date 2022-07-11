#pragma once

#include <iostream>
#include <vector>
#include "shader.h"
#include "texture.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;

	static std::vector<Vertex> genList(float* vertices, unsigned int nVertices);

};
typedef Vertex Vertex;

class Mesh
{
public:
	unsigned int VAO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	
	Mesh();
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void render(Shader shader);
	void cleanup();
	void setup();
	
private:
	unsigned int VBO, EBO;
};