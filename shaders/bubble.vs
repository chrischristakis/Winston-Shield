#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

out vec3 normal;

uniform mat4 projection;
uniform mat4 modelView;

void main() {
	mat3 normalMat = mat3(transpose(inverse(modelView)));
	normal = normalMat * inNormal;

	gl_Position = projection * modelView * vec4(inPos, 1.0);
}