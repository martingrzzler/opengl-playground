#include "PointLight.h"

PointLight::PointLight() : Light()
{
	_position = glm::vec3(0.0f);
	_constant = 0.0f;
	_linear = 0.0f;
	_exponent = 0.0f;
}

// PointLight::PointLight(PointLight &&other) noexcept
// {
// 	std::cout << "Move contructor called" << std::endl;
// }

// PointLight::PointLight(const PointLight &other)
// {
// 	std::cout << "Copy contructor" << std::endl;
// }

// PointLight &PointLight::operator=(PointLight &other)
// {
// 	std::cout << "Copy Assign contructor" << std::endl;
// 	return other;
// }

// PointLight &PointLight::operator=(PointLight &&other)
// {
// 	std::cout << "Move Assign contructor" << std::endl;
// 	return other;
// }

void PointLight::use(GLuint ambient_intensity_loc,
										 GLuint color_loc,
										 GLuint diffuse_intensity_loc,
										 GLuint position_loc,
										 GLuint constant_loc,
										 GLuint linear_loc,
										 GLuint exponent_loc)
{
	glUniform3f(color_loc, _color.x, _color.y, _color.z);
	glUniform1f(ambient_intensity_loc, _ambient_intensity);
	glUniform1f(diffuse_intensity_loc, _diffuse_intensity);
	glUniform3f(position_loc, _position.x, _position.y, _position.z);
	glUniform1f(constant_loc, _constant);
	glUniform1f(linear_loc, _linear);
	glUniform1f(exponent_loc, _exponent);
}

PointLightBuilder::operator PointLight() const
{
	return std::move(_light);
}

PointLight::~PointLight()
{
}

PointLightBuilder PointLight::builder()
{
	return PointLightBuilder();
}

PointLightBuilder::PointLightBuilder()
{
	_light = PointLight();
}

PointLightBuilder &PointLightBuilder::color(GLfloat r, GLfloat g, GLfloat b)
{
	_light._color = glm::vec3(r, g, b);
	return *this;
}

PointLightBuilder &PointLightBuilder::ambient_intensity(GLfloat ambient_intensity)
{
	_light._ambient_intensity = ambient_intensity;
	return *this;
}
PointLightBuilder &PointLightBuilder::diffuse_intensity(GLfloat diffuse_intensity)
{
	_light._diffuse_intensity = diffuse_intensity;
	return *this;
}

PointLightBuilder &PointLightBuilder::position(GLfloat x, GLfloat y, GLfloat z)
{
	_light._position = glm::vec3(x, y, z);
	return *this;
}

PointLightBuilder &PointLightBuilder::attenuation(GLfloat constant, GLfloat linear, GLfloat expo)
{
	_light._constant = constant;
	_light._linear = linear;
	_light._exponent = expo;
	return *this;
}
