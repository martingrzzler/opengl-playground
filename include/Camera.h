#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat movement_speed, GLfloat turn_speed);
	void key_control(bool *keys, GLfloat delta);
	void mouse_control(GLfloat x_change, GLfloat y_change);
	glm::mat4 view_matrix();
	glm::vec3 position();
	glm::vec3 direction();
	~Camera();

private:
	glm::vec3 _position;
	glm::vec3 _front;
	glm::vec3 _up;
	glm::vec3 _right;
	glm::vec3 _world_up;

	GLfloat _pitch;
	GLfloat _yaw;

	GLfloat _movement_speed;
	GLfloat _mouse_sensitivity;

	void update();
};