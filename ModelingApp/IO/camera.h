#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraDirection
{
	NONE = 0,
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera
{
public:
	Camera(glm::vec3 pos);

	glm::mat4 getViewMatrix();
	void updateCameraPos(CameraDirection dir, double dt);
	void updateCameraDirection(double dx, double dy);
	void updateCameraZoom(double dy);
	float getFov();

private:
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 target;
	glm::vec3 worldUp;
	glm::vec3 right;
	glm::vec3 up;

	float yaw;
	float pitch;

	float fov;
	float speed;

	void updateCameraVectors();
};