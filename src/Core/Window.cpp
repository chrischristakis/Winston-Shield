#include "Window.h"
#include <iostream>

// TODO: remove into a key input class
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Window::Window(int width, int height, std::string title) {

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    m_params.width = width;
    m_params.height = height;

    // Center window on screen
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int screenWidth = mode->width;
    int screenHeight = mode->height;
    glfwSetWindowMonitor(window, NULL, (screenWidth / 2) - (width / 2), (screenHeight / 2) - (height / 2), width, height, GLFW_DONT_CARE);

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Store params in the window pointer GLFW provides for us for use in lambdas
    glfwSetWindowUserPointer(window, &m_params);

    glfwSetWindowSizeCallback(window, 
        [](GLFWwindow* window, int width, int height) {
            WindowParams& params = *static_cast<WindowParams*>(glfwGetWindowUserPointer(window));
            params.width = width;
            params.height = height;

            glViewport(0, 0, width, height);
        });

    m_windowPtr = window;
}

void Window::Update() {
    glfwSwapBuffers(m_windowPtr);
    glfwPollEvents();
}

Window::~Window() {
    glfwDestroyWindow(m_windowPtr);
    glfwTerminate();
}

double Window::GetTime() {
    return glfwGetTime();
}

void Window::SetVSync(bool val) {
    val ? glfwSwapInterval(1) : glfwSwapInterval(0);
}

void Window::SetTitle(std::string title) {
    glfwSetWindowTitle(m_windowPtr, title.c_str());
}

bool Window::IsOpen() {
    return !glfwWindowShouldClose(m_windowPtr);
}

GLFWwindow* Window::GetPtr() {
    return m_windowPtr;
}

float Window::GetAspect() {
    return (float)m_params.width / m_params.height;
}
