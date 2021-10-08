#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat r,
									 GLfloat g,
									 GLfloat b,
									 GLfloat amb_intensity,
									 GLfloat dir_x,
									 GLfloat dir_y,
									 GLfloat dir_z,
									 GLfloat diffuse_intensity);

	void use(GLuint ambient_intensity_loc, GLuint color_loc, GLuint diffuse_intensity_loc, GLuint direction_loc);

	~DirectionalLight();
	friend class DirectionalLightBuilder;
	static DirectionalLightBuilder builder();

private:
	glm::vec3 _direction;
};

class DirectionalLightBuilder
{
public:
	DirectionalLightBuilder();
	operator DirectionalLight() const;
	DirectionalLightBuilder &color(GLfloat r, GLfloat g, GLfloat b);
	DirectionalLightBuilder &ambient_intensity(GLfloat ambient_intensity);
	DirectionalLightBuilder &diffuse_intensity(GLfloat diffuse_intensity);
	DirectionalLightBuilder &direction(GLfloat x, GLfloat y, GLfloat z);

private:
	DirectionalLight _light;
};
