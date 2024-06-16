#pragma once

#include "Input.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>

class Camera {
private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;

	glm::mat4 m_view, m_proj;

	float m_aspect = 0;
	float m_fov = 45.0f;

	float m_pitch = 0, m_yaw = 0;

	const float SENS = 0.035f;
	const glm::vec3 INITIAL_LOOK_DIR = { 0, 0, -1 };

	void RecalculateFrontVector();
	void ProcessMouse(Input& input);

	void RecalculateProjection();

public:

	Camera(glm::vec3 startPos, float aspect, float pitch = 0, float yaw = 0, glm::vec3 camUp = { 0, 1, 0 });

	void SetAspect(float w, float h);

	glm::mat4 GetViewProjection() const;
	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;
	glm::vec3 GetPosition() const { return m_position; }

	void Update(float dt);
};