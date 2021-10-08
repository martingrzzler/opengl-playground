#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	_direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(
		GLfloat r,
		GLfloat g,
		GLfloat b,
		GLfloat amb_intensity,
		GLfloat dir_x,
		GLfloat dir_y,
		GLfloat dir_z,
		GLfloat diffuse_intensity)
		: Light(r, g, b, amb_intensity, diffuse_intensity)
{
	_direction = glm::vec3(dir_x, dir_y, dir_z);
}

void DirectionalLight::use(GLuint ambient_intensity_loc, GLuint color_loc, GLuint diffuse_intensity_loc, GLuint direction_loc) const
{
	glUniform3f(color_loc, _color.x, _color.y, _color.z);
	glUniform1f(ambient_intensity_loc, _ambient_intensity);
	glUniform1f(diffuse_intensity_loc, _diffuse_intensity);
	glUniform3f(direction_loc, _direction.x, _direction.y, _direction.z);
}

DirectionalLight::~DirectionalLight()
{
}

// Builder
DirectionalLightBuilder::operator DirectionalLight() const
{
	return std::move(_light);
}

DirectionalLightBuilder DirectionalLight::builder()
{
	return DirectionalLightBuilder();
}

DirectionalLightBuilder::DirectionalLightBuilder()
{
	_light = DirectionalLight();
}

DirectionalLightBuilder &DirectionalLightBuilder::color(GLfloat r, GLfloat g, GLfloat b)
{
	_light._color = glm::vec3(r, g, b);
	return *this;
}

DirectionalLightBuilder &DirectionalLightBuilder::ambient_intensity(GLfloat ambient_intensity)
{
	_light._ambient_intensity = ambient_intensity;
	return *this;
}
DirectionalLightBuilder &DirectionalLightBuilder::diffuse_intensity(GLfloat diffuse_intensity)
{
	_light._diffuse_intensity = diffuse_intensity;
	return *this;
}

DirectionalLightBuilder &DirectionalLightBuilder::direction(GLfloat x, GLfloat y, GLfloat z)
{
	_light._direction = glm::vec3(x, y, z);
	return *this;
}
