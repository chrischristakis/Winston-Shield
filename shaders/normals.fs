#version 330 core

out vec4 FragCol;

uniform vec3 color;

void main() {
	FragCol = vec4(color, 1);
}