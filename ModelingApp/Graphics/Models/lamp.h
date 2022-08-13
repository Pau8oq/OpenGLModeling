#pragma once
#include "cube.h"
#include "../light.h"

class Lamp :public Cube
{
public:
	glm::vec3 lightColor;

	PointLight pointLight;

	Lamp() 
	{
		lightColor = glm::vec3(1.0f);
	}

	Lamp(glm::vec3 lightColor,
		PointLight _pointLight,
		glm::vec3 pos = glm::vec3(0.0f),
		glm::vec3 size = glm::vec3(1.0f))
		:lightColor(lightColor),
		Cube(Material::white_plastic, pos, size) 
	{
		pointLight = _pointLight;
		pointLight.position = pos;
	}

	void render(Shader shader) override
	{
		shader.set3Float("lightColor", lightColor);

		Cube::render(shader);
	}

};