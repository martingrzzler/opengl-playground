#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movement_speed, GLfloat mouse_sensitivity)
		: _position(position), _world_up(up), _yaw(yaw), _pitch(pitch), _movement_speed(movement_speed), _mouse_sensitivity(mouse_sensitivity)
{
	// by default OpenGL Camera looks down the negative z-axis
	_front = glm::vec3(0.0f, 0.0f, -1.0f);
	update();
}

void Camera::mouse_control(GLfloat x_change, GLfloat y_change)
{
	_yaw += (_mouse_sensitivity * x_change);
	_pitch += (_mouse_sensitivity * y_change);

	if (_pitch > 89.0f)
	{
		_pitch = 89.0f;
	}

	if (_pitch < -89.0f)
	{
		_pitch = -89.0f;
	}

	update();
}
void Camera::update()
{
	_front.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front.y = sin(glm::radians(_pitch));
	_front.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	_front = glm::normalize(_front);

	_right = glm::normalize(glm::cross(_front, _world_up));
	_up = glm::normalize(glm::cross(_right, _front));
}

glm::mat4 Camera::view_matrix()
{
	return glm::lookAt(_position, _position + _front, _up);
}

glm::vec3 Camera::position()
{
	return _position;
}

void Camera::key_control(bool *keys, GLfloat delta)
{
	GLfloat speed = _movement_speed * delta;

	if (keys[GLFW_KEY_W])
	{
		_position += _front * speed;
	}
	if (keys[GLFW_KEY_S])
	{
		_position -= _front * speed;
	}
	if (keys[GLFW_KEY_D])
	{
		_position += _right * speed;
	}
	if (keys[GLFW_KEY_A])
	{
		_position -= _right * speed;
	}
}

Camera::~Camera()
{
}
