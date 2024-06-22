#version 330 core

layout (location = 0) in vec3 inPos;

uniform mat4 projection;
uniform mat4 modelView;

void main() {
	gl_Position = projection * modelView * vec4(inPos, 1.0);
}