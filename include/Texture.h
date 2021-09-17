#pragma once

#include <GL/glew.h>

class Texture
{
public:
	Texture();
	Texture(const char *file_path);

	void load();
	void use();

	~Texture();
	GLuint _id;

private:
	int _width, _height, _bit_depth;
	const char *_file_path;

	void clear();
	void bind();
	void unbind();
};