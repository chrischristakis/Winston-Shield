#pragma once

#include <string>
#include <vector>
#include <assimp/scene.h>
#include "Mesh.h"

class Model {
private:

	//void ProcessNode(aiNode* node, const aiScene* scene);

	std::vector<Mesh> m_meshes;

	void AddMesh(aiMesh*);

public:

	Model(const std::string& path);

	void Draw(Shader& shader);

};