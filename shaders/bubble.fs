#version 330 core

out vec4 FragCol;

in vec3 normal;
in vec3 fragPos;

uniform sampler2D depthTexture;

uniform vec3 color;
uniform float alpha;

uniform vec2 screensize;
uniform float near;
uniform float far;

float LinearizeDepth(float depth) {
	float zNdc = 2 * depth - 1;
	float zEye = (2 * far * near) / ((far + near) - zNdc * (far - near));
	float linearDepth = (zEye - near) / (far - near);
	return linearDepth;
}

void main() {

	// - Rim lighting -
	vec3 viewDir = normalize(-fragPos);
	// The more orthogonal the camera is to the fragment, the stronger the rim light.
	// abs() so that the back faces get treated the same as the front, giving a rim effect.
	float rimStrength = max(0, 1 - abs(dot(viewDir, normal))); // The more orthogonal, the stronger
	float rimFactor = pow(rimStrength, 4); // higher power = sharper rim light
	vec4 rim = vec4(rimFactor);

	// - Create the intersection line -

	// Turn frag coord from screenspace -> NDC, which corresponds to the UV
	vec2 depthUV = gl_FragCoord.xy / screensize;
	float topographyDepth = texture(depthTexture, depthUV).r;
	float bubbleDepth = LinearizeDepth(gl_FragCoord.z);

	float threshold = 0.001;
	float distance = abs(bubbleDepth - topographyDepth); 
	float normalizedDistance = clamp(distance / threshold, 0.0, 1.0); // [0, 1]

	vec4 target = vec4(color, alpha);
	vec4 intersection = mix(vec4(1), target, normalizedDistance); // white to target gradient

	// Gradient from white to target colour
	FragCol = intersection + rim;
}