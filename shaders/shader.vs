#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

out vec3 normal;
out vec3 lightDirection;

uniform mat4 projection;
uniform mat4 modelView;
uniform mat4 view;
uniform vec3 lightDir;

void main() {
	mat3 normalMat = mat3(transpose(inverse(modelView))); // Add view * model when model matrix added
	normal = normalMat * inNormal;
	lightDirection = mat3(view) * lightDir;

	gl_Position = projection * modelView * vec4(inPos, 1.0);
}