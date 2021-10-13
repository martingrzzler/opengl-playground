#include "SpotLight.h"

SpotLight::SpotLight() : PointLight()
{
	_edge = 0.0f;
	_direction = glm::vec3(0.0f, 0.0f, 0.0f);
}

void SpotLight::position(glm::vec3 position)
{
	_position = position;
}

void SpotLight::direction(glm::vec3 direction)
{
	_direction = direction;
}

void SpotLight::use(GLuint ambient_intensity_loc,
										GLuint color_loc,
										GLuint diffuse_intensity_loc,
										GLuint position_loc,
										GLuint constant_loc,
										GLuint linear_loc,
										GLuint exponent_loc,
										GLuint direction_loc,
										GLuint edge_loc)
{
	glUniform3f(color_loc, _color.x, _color.y, _color.z);
	glUniform1f(ambient_intensity_loc, _ambient_intensity);
	glUniform1f(diffuse_intensity_loc, _diffuse_intensity);
	glUniform3f(position_loc, _position.x, _position.y, _position.z);
	glUniform1f(constant_loc, _constant);
	glUniform1f(linear_loc, _linear);
	glUniform1f(exponent_loc, _exponent);
	glUniform3f(direction_loc, _direction.x, _direction.y, _direction.z);
	glUniform1f(edge_loc, _edge);
}

SpotLightBuilder::operator SpotLight() const
{
	return std::move(_light);
}

SpotLight::~SpotLight()
{
}

SpotLightBuilder SpotLight::builder()
{
	return SpotLightBuilder();
}

SpotLightBuilder::SpotLightBuilder()
{
	_light = SpotLight();
}

SpotLightBuilder &SpotLightBuilder::color(GLfloat r, GLfloat g, GLfloat b)
{
	_light._color = glm::vec3(r, g, b);
	return *this;
}

SpotLightBuilder &SpotLightBuilder::ambient_intensity(GLfloat ambient_intensity)
{
	_light._ambient_intensity = ambient_intensity;
	return *this;
}
SpotLightBuilder &SpotLightBuilder::diffuse_intensity(GLfloat diffuse_intensity)
{
	_light._diffuse_intensity = diffuse_intensity;
	return *this;
}

SpotLightBuilder &SpotLightBuilder::edge(GLfloat edge)
{
	_light._edge = cosf(glm::radians(edge));
	return *this;
}

SpotLightBuilder &SpotLightBuilder::position(GLfloat x, GLfloat y, GLfloat z)
{
	_light._position = glm::vec3(x, y, z);
	return *this;
}
SpotLightBuilder &SpotLightBuilder::direction(GLfloat x, GLfloat y, GLfloat z)
{
	_light._direction = glm::normalize(glm::vec3(x, y, z));
	return *this;
}

SpotLightBuilder &SpotLightBuilder::attenuation(GLfloat constant, GLfloat linear, GLfloat expo)
{
	_light._constant = constant;
	_light._linear = linear;
	_light._exponent = expo;
	return *this;
}
