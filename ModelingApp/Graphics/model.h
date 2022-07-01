#pragma once

#include "mesh.h"
#include <vector>

class Model
{
public:
	std::vector<Mesh> meshes;

	Model();

	void init();
	void render(Shader shader);
	void cleanup();
};