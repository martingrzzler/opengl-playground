#include "Light.h"
#include <glm/gtx/string_cast.hpp>
#include <iostream>

Light::Light()
{
	_color = glm::vec3(1.0f, 1.0f, 1.0f);
	_ambient_intensity = 1.0f;
	_diffuse_intensity = 0.0f;
}

Light::Light(GLfloat r, GLfloat g, GLfloat b, GLfloat amb_intensity, GLfloat diffuse_intensity)
{
	_color = glm::vec3(r, g, b);
	_ambient_intensity = amb_intensity;
	_diffuse_intensity = diffuse_intensity;
}

// void Light::use(GLuint ambient_intensity_loc, GLuint color_loc, GLuint diffuse_intensity_loc)
// {
// 	glUniform3f(color_loc, _color.x, _color.y, _color.z);
// 	glUniform1f(ambient_intensity_loc, _ambient_intensity);
// 	glUniform1f(diffuse_intensity_loc, _diffuse_intensity);
// }

Light::~Light()
{
}