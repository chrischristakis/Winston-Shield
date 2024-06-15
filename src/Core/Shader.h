#pragma once

#include <unordered_map>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader {
private:

	unsigned int m_ID;

	using UniformMap = std::unordered_map<std::string, GLint>; // uniform location -> address
	UniformMap m_uniformLocations;

	void CreateProgram(const std::vector<unsigned int>& shaders);

public:

	// TODO: Extend to support geometry shaders and tesselation.
	Shader(const std::string& vsPath, const std::string& fsPath);
	Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath);
	~Shader();

	Shader& Use();

	unsigned int CreateShader(GLenum type, const std::string& srcPath);

	GLint GetUniformLocation(const std::string& name);
	void SetMatrix4f(const std::string& name, const glm::mat4& val);
	void SetVec4f(const std::string& name, const glm::vec4& val);
	void SetVec3f(const std::string& name, const glm::vec3& val);
	void SetVec2f(const std::string& name, const glm::vec2& val);
	void SetInt(const std::string& name, int val);
	void SetFloat (const std::string& name, float val);

};