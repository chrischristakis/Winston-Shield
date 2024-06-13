#pragma once

#include <glm/glm.hpp>

class Input {
public:

    bool IsKeyPressed(int keyCode);
    bool IsMouseButtonPressed(int mouseButton);

    glm::vec2 GetMousePos();

};