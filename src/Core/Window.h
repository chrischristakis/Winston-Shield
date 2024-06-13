#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window {
public:

	Window(int width, int height, std::string title = "Game");
	~Window();

	void Update();

	void SetVSync(bool);
	void SetTitle(std::string);

	double GetTime(); // Gets seconds since program started
	GLFWwindow* GetPtr();
	glm::vec2 GetSize() { return { m_params.width, m_params.height }; }
	float GetAspect();
	
	bool IsOpen();

private:

	GLFWwindow* m_windowPtr = nullptr;

	struct WindowParams {
		int width, height;
	} m_params;

};