#pragma once

#include <GL/glew.h>

class Material
{
public:
	Material();
	Material(GLfloat spec_inten, GLfloat shininess);

	void use(GLuint specular_intensity_loc, GLuint shininess_loc);

	~Material();

private:
	GLfloat _specular_intensity;
	GLfloat _shininess;
};