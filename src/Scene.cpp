#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <vector>
#include "Game.h"
#include "Core/Shader.h"
#include "Core/Mesh.h"
#include "Core/Model.h"

std::unique_ptr<Shader> normalsShader;
std::unique_ptr<Shader> shader;
std::unique_ptr<Model> sphere;

Scene::Scene() {
	normalsShader = std::make_unique<Shader>("shaders/normals.vs", "shaders/normals.fs", "shaders/normals.gs");
	shader = std::make_unique<Shader>("shaders/shader.vs", "shaders/shader.fs");
	sphere = std::make_unique<Model>("assets/sphere.obj");
}

void Scene::Update(float dt) {
	
}

static void RenderSphere(bool normals=false) {

	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians((float)glfwGetTime() * 100), glm::vec3(1, 0, 0));

	shader->Use();
	shader->SetMatrix4f("modelView", Game::GetCamera().GetView() * model);
	shader->SetMatrix4f("view", Game::GetCamera().GetView());
	shader->SetMatrix4f("projection", Game::GetCamera().GetProjection());
	shader->SetVec3f("color", { 1, 1, 1 });
	shader->SetVec3f("lightDir", { 0.5f, -0.5f, -0.5f });

	sphere->Draw(*shader);

	if (normals) {
		normalsShader->Use();
		normalsShader->SetMatrix4f("modelView", Game::GetCamera().GetView() * model);
		normalsShader->SetMatrix4f("view", Game::GetCamera().GetView());
		normalsShader->SetMatrix4f("projection", Game::GetCamera().GetProjection());
		normalsShader->SetVec3f("color", { 1, 1, 1 });

		sphere->Draw(*normalsShader);
	}
}

void Scene::Render() {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	RenderSphere(true);
}