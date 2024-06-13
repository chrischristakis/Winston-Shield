#pragma once

#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/Camera.h"
#include "Scene.h"
#include <memory>
#include <iostream>

#define print(msg) std::cout << msg << std::endl; 

class Game {
private:

	static std::unique_ptr<Window> m_window;
	static std::unique_ptr<Input> m_input;
	static std::unique_ptr<Camera> m_camera;

	static std::unique_ptr<Scene> m_scene;

public:

	Game();
	void Run();

	void Update(float dt);
	void Render();

	static Window& GetWindow() { return *m_window; }
	static Input& GetInput() { return *m_input; }
	static Camera& GetCamera() { return *m_camera; }
	static Scene& GetScene() { return *m_scene; }

};