#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "mesh.h"

class Model
{
public:
	glm::vec3 pos;
	glm::vec3 size;
	std::vector<Mesh> meshes;

	Model(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f));

	virtual void init();
	virtual void render(Shader shader);
	void cleanup();

};