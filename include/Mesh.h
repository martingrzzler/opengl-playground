#pragma once

#include <GL/glew.h>

class Mesh
{
public:
	Mesh();

	void create(GLfloat *vertices, unsigned int *indices, unsigned int num_of_vertices, unsigned int num_of_indices);
	void render();
	void clear();

	~Mesh();
	///

private:
	GLuint VAO, VBO, IBO;
	GLsizei index_count;

	void bind();
	void unbind();
};
