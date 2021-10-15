#pragma once

#include <GL/glew.h>
#include <string>

enum TextureMaterial
{
	DIFFUSE,
	SPECULAR,
};

class Texture
{
public:
	Texture(std::string directory, std::string file_name, TextureMaterial type);

	TextureMaterial type();
	std::string file_name();

	void use();

	~Texture();

	static std::string texture_type_to_string(TextureMaterial type);

private:
	void _load();
	std::string _file_name;
	std::string _directory;
	GLuint _id;
	TextureMaterial _type;

	void clear();
	void bind();
	void unbind();
};