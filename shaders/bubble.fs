#version 330 core

out vec4 FragCol;

in vec3 normal;

uniform sampler2D depthTexture;

uniform vec3 color;
uniform vec2 screensize;
uniform float alpha;

uniform float near;
uniform float far;

float LinearizeDepth(float depth) {
	float zNdc = 2 * depth - 1;
	float zEye = (2 * far * near) / (zNdc * (far - near) - (far + near));
	zEye *= -1;
	float linearDepth = (zEye - near) / (far - near);
	return linearDepth;
}

void main() {
	vec2 depthUV = gl_FragCoord.xy / screensize;

	float topographyDepth = texture(depthTexture, depthUV).r;
	float bubbleDepth = LinearizeDepth(gl_FragCoord.z);

	if (abs(bubbleDepth - topographyDepth) < 0.001)
		FragCol = vec4(1, 1, 1, 1);
	else
		FragCol = vec4(color, alpha);
}