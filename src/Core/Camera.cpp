#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "Constants.h"

void Camera::RecalculateFrontVector() {
	// Prevent 'flipping' camera
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::mat4 yawMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 pitchMat = glm::rotate(glm::mat4(1.0f), glm::radians(m_pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::vec4 lookDir = yawMat * pitchMat * glm::vec4(INITIAL_LOOK_DIR, 1);

	m_front = glm::normalize(glm::vec3(lookDir));
}


Camera::Camera(glm::vec3 startPos, float aspect, float pitch, float yaw, glm::vec3 camUp) :
	m_aspect(aspect), m_pitch(pitch), m_yaw(yaw), m_up(camUp),
	m_position(startPos), m_front(INITIAL_LOOK_DIR),
	m_right(glm::normalize(glm::cross(m_front, camUp)))
{
	RecalculateFrontVector();
	RecalculateProjection();
}

void Camera::SetAspect(float w, float h) {
	m_aspect = w / h;
	RecalculateProjection();
}

glm::mat4 Camera::GetViewProjection() const {
	return m_proj * m_view;
}

glm::mat4 Camera::GetView() const {
	return m_view;
}

glm::mat4 Camera::GetProjection() const {
	return m_proj;
}

void Camera::Update(float dt) {
	Input& input = Game::GetInput();

	ProcessMouse(input);

	const float SPEED = 5.0f * dt;
	m_right = glm::normalize(glm::cross(m_front, m_up));
	if (input.IsKeyPressed(GLFW_KEY_W))
		m_position += m_front * SPEED;
	if (input.IsKeyPressed(GLFW_KEY_S))
		m_position -= m_front * SPEED;
	if (input.IsKeyPressed(GLFW_KEY_A))
		m_position -= m_right * SPEED;
	if (input.IsKeyPressed(GLFW_KEY_D))
		m_position += m_right * SPEED;

	// Up and down movement
	if (input.IsKeyPressed(GLFW_KEY_SPACE))
		m_position += m_up * SPEED;
	if (input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
		m_position -= m_up * SPEED;

	m_view = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::ProcessMouse(Input& input) {
	static float prevX = 0, prevY = 0;
	static bool firstMouseMove = true;

	glm::vec2 mpos = input.GetMousePos();

	if (firstMouseMove) {
		prevX = mpos.x;
		prevY = mpos.y;
		firstMouseMove = false;
	}

	float offsetX = mpos.x - prevX;
	float offsetY = mpos.y - prevY;

	m_pitch -= offsetY * SENS;
	m_yaw -= offsetX * SENS;

	RecalculateFrontVector();

	prevX = mpos.x;
	prevY = mpos.y;
}

void Camera::RecalculateProjection() {
	m_proj = glm::perspective(m_fov, m_aspect, NEAR, FAR);
}
