#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat r, GLfloat g, GLfloat b, GLfloat amb_intensity, GLfloat dir_x, GLfloat dir_y, GLfloat dir_z, GLfloat dir_intensity);

	void use(GLuint ambient_intensity_loc, GLuint color_loc, GLuint diffuse_intensity_loc, GLuint direction_loc);

	~Light();

private:
	glm::vec3 _color;
	GLfloat _ambient_intensity;
	glm::vec3 _direction;
	GLfloat _diffuse_intensity;
};