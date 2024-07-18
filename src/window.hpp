#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

    
class Window {
public:
    Window();
    ~Window();

    static Window* createWindow(const int w, const int h, const char* t, bool fs);
    GLFWwindow* getWindow();

    void useMainCallbacks();
    static void freeWindow(Window* window);
    bool shouldClose();
    void close();

private:
    GLFWwindow* m_window;

};
