#version 330 core

out vec4 FragCol;

uniform sampler2D tex;

in vec2 uv;

void main() {
	FragCol = texture(tex, uv);
}