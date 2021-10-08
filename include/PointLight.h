#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	// PointLight(PointLight &&other) noexcept;
	// PointLight(const PointLight &other);
	// PointLight &operator=(PointLight &other);
	// PointLight &operator=(PointLight &&other);

	~PointLight();
	friend class PointLightBuilder;
	static PointLightBuilder builder();

private:
	glm::vec3 _position;
	GLfloat _constant, _linear, _exponent;
};

class PointLightBuilder
{
public:
	PointLightBuilder();
	operator PointLight() const;
	PointLightBuilder &color(GLfloat r, GLfloat g, GLfloat b);
	PointLightBuilder &ambient_intensity(GLfloat ambient_intensity);
	PointLightBuilder &diffuse_intensity(GLfloat diffuse_intensity);
	PointLightBuilder &position(GLfloat x, GLfloat y, GLfloat z);
	PointLightBuilder &attenuation(GLfloat constant, GLfloat linear, GLfloat expo);

private:
	PointLight _light;
};