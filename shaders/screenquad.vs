#version 330 core

layout (location = 0) in vec3 inPos;
layout (location = 2) in vec2 inUv;

out vec2 uv;

void main() {
	gl_Position = vec4(inPos, 1.0);
	uv = inUv;
}