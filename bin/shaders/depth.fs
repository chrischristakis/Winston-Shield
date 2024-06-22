#version 330 core

uniform float near;
uniform float far;

float LinearizeDepth(float depth) {
	float zNdc = 2 * depth - 1;
	float zEye = (2 * far * near) / ((far + near) - zNdc * (far - near));
	float linearDepth = (zEye - near) / (far - near);
	return linearDepth;
}

void main() {
	gl_FragDepth = LinearizeDepth(gl_FragCoord.z);
}