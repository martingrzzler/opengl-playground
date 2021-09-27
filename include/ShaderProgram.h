#pragma once

#include <GL/glew.h>
#include <iostream>

class ShaderProgram
{
public:
	ShaderProgram();
	void create_from_string(const char *v_shader_src, const char *f_shader_src);
	void create_from_file(const char *v_shader_path, const char *f_shader_path);
	GLuint projection_location();
	GLuint model_location();
	GLuint view_location();
	GLuint ambient_intensity_location();
	GLuint ambient_color_location();
	void use();
	void unuse();
	void clear();
	~ShaderProgram();

private:
	GLuint _program_id, _uniform_projection, _uniform_model,
			_uniform_view, _uniform_ambient_intensity, _uniform_ambient_color;

	void compile_shader(const char *shader_src, GLenum type);
	void build(const char *v_shader_src, const char *f_shader_src);
};