#version 330 core

uniform float near;
uniform float far;

float LinearizeDepth(float depth) {
	// Turn back into NDC
	float zNdc = 2 * depth - 1;

	// Turn ndc back to eye space
	float zEye = (2 * far * near) / (zNdc * (far - near) - (far + near));
	
	// Since z is negative (camera looks down -z, view transforms things to be infront of camera)
	// we must negate zEye to be positive to map to [0, 1] since near and far are also positive
	zEye *= -1;

	// Map back to [0, 1] using linear normalization, 0 being close to near, 1 close to far
	float linearDepth = (zEye - near) / (far - near);

	return linearDepth;
}

void main() {
	gl_FragDepth = LinearizeDepth(gl_FragCoord.z);
}