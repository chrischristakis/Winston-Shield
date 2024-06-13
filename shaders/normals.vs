#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

out vec3 normal;

uniform mat4 view;
uniform mat4 modelView;

void main() {
	gl_Position = modelView * vec4(inPos, 1.0);
	mat3 normalMat = mat3(transpose(inverse(modelView))); // Add view * model when that's a thing
	normal = normalize(normalMat * inNormal);
}