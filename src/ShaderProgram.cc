#include "ShaderProgram.h"
#include "utils.h"

ShaderProgram::ShaderProgram()
{
	_program_id = _uniform_model = _uniform_projection = _uniform_view = 0;
}

GLuint ShaderProgram::model_location()
{
	return _uniform_model;
}

GLuint ShaderProgram::projection_location()
{
	return _uniform_projection;
}

GLuint ShaderProgram::view_location()
{
	return _uniform_view;
}

void ShaderProgram::create_from_string(const char *v_shader_src, const char *f_shader_src)
{
	build(v_shader_src, f_shader_src);
}

void ShaderProgram::create_from_file(const char *v_shader_path, const char *f_shader_path)
{
	try
	{
		auto v_shader_src = utils::read_file_to_string(v_shader_path);
		auto f_shader_src = utils::read_file_to_string(f_shader_path);
		build(v_shader_src.c_str(), f_shader_src.c_str());
	}
	catch (const char *e)
	{
		std::cerr << e << '\n';
		throw e;
	}
}

void ShaderProgram::build(const char *v_shader_src, const char *f_shader_src)
{
	_program_id = glCreateProgram();
	if (!_program_id)
	{
		std::cerr << "ERROR: failed to create shader program" << std::endl;
		return;
	}

	compile_shader(v_shader_src, GL_VERTEX_SHADER);
	compile_shader(f_shader_src, GL_FRAGMENT_SHADER);

	GLint res = 0;
	GLchar msg[1024] = {0};
	glLinkProgram(_program_id);
	glGetProgramiv(_program_id, GL_LINK_STATUS, &res);
	if (res == GL_FALSE)
	{
		glGetProgramInfoLog(_program_id, sizeof(msg), NULL, msg);
		std::cerr << "ERROR: Linking shaders - " << msg << std::endl;
		return;
	}
	glValidateProgram(_program_id);
	glGetProgramiv(_program_id, GL_VALIDATE_STATUS, &res);
	if (res == GL_FALSE)
	{
		glGetProgramInfoLog(_program_id, sizeof(msg), NULL, msg);
		std::cerr << "ERROR: Validating shaders - " << msg << std::endl;
		return;
	}
	_uniform_model = glGetUniformLocation(_program_id, "model");
	_uniform_projection = glGetUniformLocation(_program_id, "projection");
	_uniform_view = glGetUniformLocation(_program_id, "view");
}

void ShaderProgram::compile_shader(const char *shader_src, GLenum type)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &shader_src, NULL);
	glCompileShader(shader);

	GLint res;
	char msg[1024];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
	if (res == GL_FALSE)
	{
		glGetShaderInfoLog(shader, sizeof(msg), NULL, msg);
		std::cerr << "ERROR: Compiling shader " << type << "failed with message\n"
							<< msg << std::endl;
		return;
	}
	glAttachShader(_program_id, shader);
}

void ShaderProgram::use()
{
	glUseProgram(_program_id);
}
void ShaderProgram::unuse()
{
	glUseProgram(0);
}

void ShaderProgram::clear()
{
	if (_program_id != 0)
	{
		glDeleteProgram(_program_id);
		_program_id = 0;
	}
	_uniform_model = 0;
	_uniform_projection = 0;
}

ShaderProgram::~ShaderProgram()
{
	clear();
}