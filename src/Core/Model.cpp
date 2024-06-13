#include "Model.h"
#include "Game.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string& path) {

	// Load in model using Assimp's importer, aiProcess_Triangulate
	// tells it to turn any non-triangles in the model into triangles
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	if (!scene) {
		print("Error loading model '" << path << "': " << importer.GetErrorString());
		return;
	}

	for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		AddMesh(scene->mMeshes[i]);
}

void Model::Draw(Shader& shader) {
	for (Mesh& mesh : m_meshes)
		mesh.Draw(shader);
}

void Model::AddMesh(aiMesh* mesh) {
	std::vector<Vertex> vertices;
	std::vector<Index> inds;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// All vertices have a position
		vertex.pos = {
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		};

		// A vertex may have a normal
		if (mesh->mNormals) {
			vertex.normal = {
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};
		}

		vertices.push_back(vertex);
	}

	// Read in index data
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			inds.push_back(face.mIndices[j]);
	}

	m_meshes.emplace_back(vertices, inds);
}