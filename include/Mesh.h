#pragma once
#include "ShaderProgram.h"
#include "Texture.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class ShaderProgram;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_cords;
};

class Mesh
{
public:
	Mesh(
			std::vector<Vertex> vertices,
			std::vector<unsigned int> indices,
			std::vector<Texture> textures);
	void draw(ShaderProgram *shader);
	~Mesh();

private:
	std::vector<Vertex> _vertices;
	std::vector<unsigned int> _indices;
	std::vector<Texture> _textures;

	void unbind();
	void bind();
	void clear();

	GLuint _vao, _vbo, _ebo;
	void create();
};