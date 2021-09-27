#pragma once

#include <gl/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat r, GLfloat g, GLfloat b, GLfloat intensity);

	void use(GLuint intensity_loc, GLuint color_loc);

	~Light();

private:
	glm::vec3 _color;
	GLfloat _intensity;
};