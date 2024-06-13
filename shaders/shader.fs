#version 330 core

out vec4 FragCol;

in vec3 normal;
in vec3 lightDirection;

uniform vec3 color;

void main() {
	float ambientStrength = 0.1;
	vec3 ambient = color * ambientStrength;

	vec3 lightDir = normalize(-lightDirection);
	float diffuseStrength = max(dot(lightDir, normal), 0);
	vec3 diffuse = color * diffuseStrength;

	FragCol = vec4((ambient + diffuse), 1);
}