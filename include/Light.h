#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat r, GLfloat g, GLfloat b, GLfloat amb_intensity, GLfloat dir_intensity);

	// virtual void use(GLuint ambient_intensity_loc, GLuint color_loc, GLuint diffuse_intensity_loc);

	~Light();

protected:
	glm::vec3 _color;
	GLfloat _ambient_intensity;
	GLfloat _diffuse_intensity;
};
