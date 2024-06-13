#include "Input.h"
#include "Game.h"

bool Input::IsKeyPressed(int keyCode) {
    return glfwGetKey(Game::GetWindow().GetPtr(), keyCode) == GLFW_PRESS;
}

bool Input::IsMouseButtonPressed(int mouseButton) {
    return glfwGetMouseButton(Game::GetWindow().GetPtr(), mouseButton) == GLFW_PRESS;
}

glm::vec2 Input::GetMousePos() {
    double xpos = 0, ypos = 0;
    glfwGetCursorPos(Game::GetWindow().GetPtr(), &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}
