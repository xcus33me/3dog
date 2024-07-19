#include "window.hpp"

// stl
#include <iostream>

Window::Window() : m_window(nullptr) {
};

Window::~Window() {
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
}

Window* Window::createWindow(const int weight, const int height, const char* title, bool fs) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        
    Window* curr = new Window();

    if (fs) {
        curr->m_window = glfwCreateWindow(
            weight, 
            height,
            title,
            glfwGetPrimaryMonitor(),
            nullptr
        );
    } else {
        curr->m_window = glfwCreateWindow(
            weight, 
            height, 
            title, 
            nullptr, 
            nullptr
        );
    }
        
    if (!curr->m_window) {
        std::cout << "Failed to create window!\n";
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(curr->m_window);

    return curr;
}

GLFWwindow* Window::getWindow() {
    return m_window;
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(m_window);
}


void Window::close() {
    if (m_window != nullptr) {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
    }
}

void Window::freeWindow(Window* window) {
    if (window) {
        glfwDestroyWindow(window->m_window);
        delete window;
    }
}

void Window::useMainCallbacks() {
    // todo
}
