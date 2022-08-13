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

enum class CameraType
{
	FPS = 0,
	ORBIT = 1
};

class Camera
{
public:
	Camera(glm::vec3 pos, CameraType cameraType = CameraType::FPS);

	glm::mat4 getViewMatrix();
	void updateCameraPos(CameraDirection dir, double dt);
	void updateCameraDirection(double dx, double dy);
	void updateCameraZoom(double dy);
	float getFov();
	glm::vec3 getPos();
	glm::vec3 getFront();

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

	CameraType cameraType;

	void updateCameraVectors();
	void updateFPSCameraVectors();
	void updateORBITCameraVectors();
	void initYaw();
	void initPitch();
};