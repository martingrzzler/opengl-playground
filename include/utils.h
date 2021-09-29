#pragma once

#include <GL/glew.h>
#include <string>

namespace utils
{
	std::string read_file_to_string(const char *path);
	void calc_avg_normals(
			unsigned int *indices,
			unsigned int indices_count,
			GLfloat *vertices,
			unsigned int vertices_count,
			unsigned int vertex_length,
			unsigned int normal_offset);
}
