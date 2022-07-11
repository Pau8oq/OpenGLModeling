#include "camera.h"
#include <iostream>

Camera::Camera(glm::vec3 pos, CameraType cameraType)
	:pos(pos), cameraType(cameraType)
{
	target = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	speed = 2.5f;
	fov = 45.0f;

	initYaw();
	initPitch();

	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	if (cameraType == CameraType::FPS)
		updateFPSCameraVectors();
	else if (cameraType == CameraType::ORBIT)
		updateORBITCameraVectors();
}


void Camera::updateORBITCameraVectors()
{
	float distance = glm::length(pos - target);

	pos.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	pos.y = -glm::sin(glm::radians(pitch));
	pos.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

	//because pos mormalized
	pos *= distance;

	front = glm::normalize(target - pos);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::updateFPSCameraVectors()
{
	glm::vec3 direction;

	direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
	direction.y = glm::sin(glm::radians(pitch));
	direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

	front = glm::normalize(direction);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::initYaw()
{
	//as we want front.z to be 1, sin(0) = 0, sin(90) = 1
	yaw = 90.0f;

	glm::vec3 default_pos(0.0f, 0.0f, pos.z);
	glm::vec3 yaw_pos(pos.x, 0.0f, pos.z);

	glm::vec3 vectorA = glm::normalize(target - yaw_pos);
	glm::vec3 vectorB = glm::normalize(target - default_pos);

	float radians = glm::acos(glm::dot(vectorA, vectorB));
	float degrees = glm::degrees(radians);

	yaw += degrees;
}

void Camera::initPitch()
{
	pitch = 0.0f;

	glm::vec3 default_pos(0.0f, 0.0f, pos.z);
	glm::vec3 pitch_pos(0.0f, pos.y, pos.z);

	glm::vec3 vectorA = glm::normalize(target - pitch_pos);
	glm::vec3 vectorB = glm::normalize(target - default_pos);

	float radians = glm::acos(glm::dot(vectorA, vectorB));
	float degrees = glm::degrees(radians);

	pitch += pos.y > 0 ? -1 * degrees : degrees;
}

void Camera::updateCameraDirection(double dx, double dy)
{
	yaw += dx;
	pitch += dy;

	if (pitch > 89.0f)
		pitch = 89.0f;
	else if (pitch < -89.0f)
		pitch = -89.0f;

	if (yaw > 360.0f || yaw < -360.0f)
		yaw = 0.0f;

	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 ret(1.0f);
	ret = glm::lookAt(pos, pos + front, up);

	return ret;
}

float Camera::getFov()
{
	return fov;
}

glm::vec3 Camera::getPos()
{
	return pos;
}

void Camera::updateCameraZoom(double dy)
{
	pos += front * (float)(dy);
}

void Camera::updateCameraPos(CameraDirection dir, double dt)
{
	float velocity = (float)dt * speed;

	switch (dir)
	{
	case CameraDirection::NONE:
		break;
	case CameraDirection::FORWARD:
		pos += front * velocity;
		break;
	case CameraDirection::BACKWARD:
		pos -= front * velocity;
		break;
	case CameraDirection::LEFT:
		pos -= right * velocity;
		break;
	case CameraDirection::RIGHT:
		pos += right * velocity;
		break;
	case CameraDirection::UP:
		pos += up * velocity;
		break;
	case CameraDirection::DOWN:
		pos -= up * velocity;
		break;
	default:
		break;
	}
	
}
