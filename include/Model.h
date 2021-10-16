#pragma once

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <memory>
#include <string>
#include <vector>

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

class Model
{
public:
	Model(const char *path);
	void draw(std::unique_ptr<ShaderProgram> &shader);
	~Model();

private:
	std::vector<std::unique_ptr<Mesh>> _meshes;
	std::string _directory;
	std::vector<std::shared_ptr<Texture>> _textures_loaded;

	void _load_model(std::string path);
	void _process_node(aiNode *node, const aiScene *scene);
	std::unique_ptr<Mesh> _process_mesh(aiMesh *mesh, const aiScene *scene);
	std::vector<std::shared_ptr<Texture>> _load_material_textures(aiMaterial *mat, aiTextureType ai_type, TextureMaterial type);
};