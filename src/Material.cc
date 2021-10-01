#include "Material.h"
#include "iostream"

Material::Material() : _shininess(0), _specular_intensity(0)
{
}

Material::Material(GLfloat spec_inten, GLfloat shininess)
		: _shininess(shininess), _specular_intensity(spec_inten)
{
}

void Material::use(GLuint specular_intensity_loc, GLuint shininess_loc)
{
	glUniform1f(specular_intensity_loc, _specular_intensity);
	glUniform1f(shininess_loc, _shininess);
}

Material::~Material()
{
}