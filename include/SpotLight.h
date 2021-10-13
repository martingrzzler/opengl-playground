#pragma once
#include "PointLight.h"

class SpotLight : public PointLight
{
public:
	SpotLight();
	void direction(glm::vec3 direction);
	void position(glm::vec3 position);
	void use(GLuint ambient_intensity_loc,
					 GLuint color_loc,
					 GLuint diffuse_intensity_loc,
					 GLuint position_loc,
					 GLuint constant_loc,
					 GLuint linear_loc,
					 GLuint exponent_loc,
					 GLuint direction_loc,
					 GLuint edge_loc);
	~SpotLight();
	friend class SpotLightBuilder;
	static SpotLightBuilder builder();

private:
	glm::vec3 _direction;
	GLfloat _edge;
};

class SpotLightBuilder
{
public:
	SpotLightBuilder();
	operator SpotLight() const;
	SpotLightBuilder &color(GLfloat r, GLfloat g, GLfloat b);
	SpotLightBuilder &ambient_intensity(GLfloat ambient_intensity);
	SpotLightBuilder &edge(GLfloat edge);
	SpotLightBuilder &diffuse_intensity(GLfloat diffuse_intensity);
	SpotLightBuilder &position(GLfloat x, GLfloat y, GLfloat z);
	SpotLightBuilder &direction(GLfloat x, GLfloat y, GLfloat z);
	SpotLightBuilder &attenuation(GLfloat constant, GLfloat linear, GLfloat expo);

private:
	SpotLight _light;
};