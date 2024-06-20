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
#include "Constants.h"
#include <cmath>

std::unique_ptr<Shader> sceneShader;
std::unique_ptr<Shader> depthShader;
std::unique_ptr<Shader> screenQuadShader;
std::unique_ptr<Shader> bubbleShader;

std::unique_ptr<Mesh> screenQuad;
std::unique_ptr<Model> bubble;
std::unique_ptr<Model> sceneModel;

glm::vec3 bubblePos(0, 0, 0);
unsigned int FBO, depthTexture;

Scene::Scene() {
	sceneShader = std::make_unique<Shader>("shaders/scene.vs", "shaders/scene.fs");
	depthShader = std::make_unique<Shader>("shaders/depth.vs", "shaders/depth.fs");
	screenQuadShader = std::make_unique<Shader>("shaders/screenquad.vs", "shaders/screenquad.fs");
	bubbleShader = std::make_unique<Shader>("shaders/bubble.vs", "shaders/bubble.fs");

	bubble = std::make_unique<Model>("assets/sphere.obj");
	sceneModel = std::make_unique<Model>("assets/scene.obj");

	// Just a quad that spans the screen to render depth texture to
	std::vector<Vertex> screenQuadVerts = {
		// pos          normals      uvs
		{{ -1, -1, 0 }, { 0, 0, 0 }, { 0, 0 }},
		{{  1, -1, 0 }, { 0, 0, 0 }, { 1, 0 }},
		{{  1,  1, 0 }, { 0, 0, 0 }, { 1, 1 }},
		{{ -1,  1, 0 }, { 0, 0, 0 }, { 0, 1 }},
	};

	std::vector<Index> screenQuadInds = {
		0, 1, 2,
		2, 3, 0
	};

	screenQuad = std::make_unique<Mesh>(screenQuadVerts, screenQuadInds);

	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	// Set up empty depth texture
	glm::vec2 windowSize = Game::GetWindow().GetSize();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowSize.x, windowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Set up framebuffer and attach depth texture
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	// No need for color attachment
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		print("Framebuffer is not complete");
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::Update(float dt) {
	static float SPEED = 2.5f;
	Input& input = Game::GetInput();
	if (input.IsKeyPressed(GLFW_KEY_LEFT))
		bubblePos.x -= SPEED * dt;
	if (input.IsKeyPressed(GLFW_KEY_RIGHT))
		bubblePos.x += SPEED * dt;
}

static void RenderBubble(Shader& shader) {
	glm::mat4 model(1.0f);

	model = glm::translate(model, bubblePos);
	glm::vec3 baseScale(2.0f);
	//glm::vec3 baseScale(0.5f * fmod(glfwGetTime() * 4, 10));
	model = glm::scale(model, baseScale);

	shader.Use();
	shader.SetMatrix4f("view", Game::GetCamera().GetView());
	shader.SetMatrix4f("model", model);
	shader.SetMatrix4f("projection", Game::GetCamera().GetProjection());
	shader.SetVec3f("color", { 0, 0.9f, 1 });
	shader.SetFloat("alpha", 0.05f);
	shader.SetVec2f("screensize", Game::GetWindow().GetSize());
	shader.SetFloat("near", NEAR);
	shader.SetFloat("far", FAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	bubble->Draw(shader);
}

static void RenderSceneDepth(Shader& shader) {
	shader.Use();
	shader.SetMatrix4f("modelView", Game::GetCamera().GetView());
	shader.SetMatrix4f("projection", Game::GetCamera().GetProjection());
	shader.SetFloat("near", NEAR);
	shader.SetFloat("far", FAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	sceneModel->Draw(shader);
}

static void RenderScene(Shader& shader) {
	shader.Use();
	shader.SetMatrix4f("modelView", Game::GetCamera().GetView());
	shader.SetMatrix4f("projection", Game::GetCamera().GetProjection());

	shader.SetMatrix4f("view", Game::GetCamera().GetView());
	shader.SetVec3f("color", { 0.3f, 0.3f, 0.3f });
	shader.SetVec3f("lightDir", { 0.5f, -0.72f, -0.65f });

	sceneModel->Draw(shader);
}

static void DrawScreenQuad() {
	screenQuadShader->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	screenQuad->Draw(*screenQuadShader);
}

void Scene::Render() {
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Draw to depth buffer (First pass)
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderSceneDepth(*depthShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw scene normally (topology)
	RenderScene(*sceneShader);

	// Draw bubble shield
	// To avoid transparency issues, we render the front and back faces
	// in two seperate calls, disabling the depth mask for both to avoid
	// any transparency issues.  
	// * Note: This means these draw calls must come after everything else,
	//         since they don't write to the depth buffer and will be overwritten.
	//         In practise, OIT or sorting faces would be more robust
	glDepthMask(GL_FALSE);
	glCullFace(GL_FRONT);
	RenderBubble(*bubbleShader);

	glCullFace(GL_BACK);
	RenderBubble(*bubbleShader);

	glDepthMask(GL_TRUE);
}	