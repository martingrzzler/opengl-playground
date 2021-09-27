#include "Light.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Light::Light()
{
	_color = glm::vec3(1.0f, 1.0f, 1.0f);
	_intensity = 1.0f;
}

Light::Light(GLfloat r, GLfloat g, GLfloat b, GLfloat intensity)
{
	_color = glm::vec3(r, g, b);
	_intensity = intensity;
}

void Light::use(GLuint intensity_loc, GLuint color_loc)
{
	glUniform3f(color_loc, _color.x, _color.y, _color.z);
	glUniform1f(intensity_loc, _intensity);
}

Light::~Light()
{
}