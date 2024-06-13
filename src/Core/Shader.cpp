#include "Shader.h"

#include <sstream>
#include <fstream>
#include <iostream>

void Shader::CreateProgram(const std::vector<unsigned int>& shaders) {
	unsigned int ID = glCreateProgram();

	for (unsigned int shader : shaders)
		glAttachShader(ID, shader);

	glLinkProgram(ID);

	int success;
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(ID, 512, NULL, log);
		std::cerr << "Error compiling program: " << log << std::endl;
		return;
	}

	// Shaders can be discarded after linking
	for (unsigned int shader : shaders)
		glDeleteShader(shader);

	m_ID = ID;
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath) {
	unsigned int vShader = CreateShader(GL_VERTEX_SHADER, vsPath);
	unsigned int fShader = CreateShader(GL_FRAGMENT_SHADER, fsPath);

	CreateProgram({ vShader, fShader });
}

Shader::Shader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath) {
	unsigned int vShader = CreateShader(GL_VERTEX_SHADER, vsPath);
	unsigned int fShader = CreateShader(GL_FRAGMENT_SHADER, fsPath);
	unsigned int gShader = CreateShader(GL_GEOMETRY_SHADER, gsPath);

	CreateProgram({ vShader, fShader, gShader });
}

Shader::~Shader() {
	glDeleteProgram(m_ID);
	m_uniformLocations.clear();
}

Shader& Shader::Use() {
	glUseProgram(m_ID);
	return *this;
}

// Wrapper to create shader given a txt file for opengl
unsigned int Shader::CreateShader(GLenum type, const std::string& srcPath) {
	std::ifstream stream(srcPath);
	std::string shaderCode;

	// Read from file into a string
	if (stream) {
		std::stringstream shaderStream;
		shaderStream << stream.rdbuf();  // Pipes shader txt into a string stream
		stream.close();
		shaderCode = shaderStream.str();
	}
	else {
		std::cerr << "Cannot read shader file at: " << srcPath << std::endl;
		return -1;
	}

	const char* srcCStr = shaderCode.c_str();

	unsigned int shaderID = glCreateShader(type);
	glShaderSource(shaderID, 1, &srcCStr, NULL);
	glCompileShader(shaderID);

	int success;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		char log[512];
		glGetShaderInfoLog(shaderID, 512, NULL, log);
		std::cerr << "Error compiling shader at path: " << srcPath << " due to: \n" << log << std::endl;
		return -1;
	}

	return shaderID;
}

GLint Shader::GetUniformLocation(const std::string& name) {
	UniformMap::iterator location = m_uniformLocations.find(name);

	// Can't find location, so create it.
	if (location == m_uniformLocations.end()) {
		GLint location = glGetUniformLocation(m_ID, name.c_str());
		m_uniformLocations.insert({ name, location });
		return location;
	}

	return location->second;
}

void Shader::SetMatrix4f(const std::string& name, const glm::mat4& val) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &val[0][0]);
}

void Shader::SetVec4f(const std::string& name, const glm::vec4& val) {
	glUniform4fv(GetUniformLocation(name), 1, &val[0]);
}

void Shader::SetVec3f(const std::string& name, const glm::vec3& val) {
	glUniform3fv(GetUniformLocation(name), 1, &val[0]);
}

void Shader::SetVec2f(const std::string& name, const glm::vec2& val) {
	glUniform2fv(GetUniformLocation(name), 1, &val[0]);
}

void Shader::SetInt(const std::string& name, int val) {
	glUniform1i(GetUniformLocation(name), val);
}