#pragma once
#include "cube.h"
#include "../light.h"

class Lamp :public Cube
{
public:
	glm::vec3 lightColor;

	PointLight pointLight;

	Lamp(glm::vec3 lightColor,
		glm::vec3 ambinet,
		glm::vec3 diffuse,
		glm::vec3 specular,
		glm::vec3 pos = glm::vec3(0.0f),
		glm::vec3 size = glm::vec3(1.0f))
		:lightColor(lightColor),
		pointLight({ pos, ambinet, diffuse, specular }),
		Cube(Material::white_plastic, pos, size) {}

	void render(Shader shader) override
	{
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader);
	}

};