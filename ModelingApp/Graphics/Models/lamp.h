#pragma once
#include "cube.h"

class Lamp :public Cube
{
public:
	glm::vec3 lightColor;

	Lamp(glm::vec3 lightColor, glm::vec3 pos = glm::vec3(0.0f), glm::vec3 size = glm::vec3(1.0f))
		:lightColor(lightColor), Cube(pos, size) {}

	void render(Shader shader) override
	{
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader);
	}
};