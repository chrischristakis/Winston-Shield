#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Shader.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 uv;
};

using Index = unsigned int;

class Mesh {
private:

	std::vector<Vertex> m_vertices;
	std::vector<Index> m_indices;

	unsigned int m_VAO, m_VBO, m_EBO;

	void InitBuffers();

public:

	Mesh(std::vector<Vertex> vertices, std::vector<Index> indices);

	void Draw(Shader& shader);

};