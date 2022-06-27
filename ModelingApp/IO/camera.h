#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	glm::vec3 pos;
	glm::vec3 dir;

	Camera(glm::vec3 pos, glm::vec3 dir);

private:
	glm::vec3 target;
	glm::vec3 dir;
	glm::vec3 worldUp;
	glm::vec3 right;
	glm::vec3 up;
};