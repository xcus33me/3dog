#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Math
#include <glm/glm.hpp>

// STL
#include <iostream>

#include "window.hpp"
#include "shader.hpp"


// callbacks
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    //std::cout << "Cursor: " << xpos << " " << ypos << std::endl;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};

const int windowWidth = 1280;
const int windowHeight = 720;
const char* winodwTitle = "3dog";

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!";
        return -1;
    }
    
    Window* window = Window::createWindow(windowWidth, windowHeight, winodwTitle, false);

    if (window == nullptr) {
        std::cout << "Failed to create window!\n";
        glfwTerminate();
        return -1;
    }

    window->useMainCallbacks();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD!";
        return -1;
    }

    glViewport(0, 0, windowWidth, windowHeight);

    // Создаем объект вертекс шейдера и получаем его ссылку.

    GLuint shaderProgram = Shader::createShaderProgram("../../shaders/vert_shader.glsl", "../../shaders/frag_shader.glsl");
    if (!shaderProgram) {
        return -1;
    }

    // Создаем VAO и VBO, загружаем vertices
    Vertex vertArray[10] = {
        //{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f) }, 
        //{ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f) }, 
        //{ glm::vec3(0.5f, 0.5f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f) }, 
        //{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f) }, 
        
        // star verticies
        { glm::vec3(-0.8f, 0.3f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f) },
        { glm::vec3(-0.3f, 0.3f, 0.0f), glm::vec4(0.8f, 0.2f, 0.9f, 1.0f)},
        { glm::vec3(0.0f, 1.0f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },
        { glm::vec3(0.3f, 0.3f, 0.0f), glm::vec4(0.9f, 0.2f, 0.8f, 1.0f) },
        { glm::vec3(0.8f, 0.3f, 0.0f), glm::vec4(0.9f, 0.2f, 0.8f, 1.0f) },
        { glm::vec3(0.4f, -0.3f, 0.0f), glm::vec4(0.1f, 0.5f, 0.8f, 1.0f) },
        { glm::vec3(0.7f, -1.0f, 0.0f), glm::vec4(0.7f, 0.7f, 0.1f, 1.0f) },
        { glm::vec3(0.0f, -0.5f, 0.0f), glm::vec4(0.1f, 0.3f, 0.8f, 1.0f) },
        { glm::vec3(-0.7f, -1.0f, 0.0f), glm::vec4(0.2f, 0.8f, 0.6f, 1.0f) },
        { glm::vec3(-0.4f, -0.3f, 0.0f), glm::vec4(0.4f, 0.9f, 0.5f, 1.0f) },
    };

    uint32_t indices[24] = {
        0, 9, 1,    1, 2, 3,    3, 4, 5,
        5, 6, 7,    7, 8, 9,    1, 3, 9,
        3, 5, 9,    5, 7, 9
    };

    // Создаем vertex array buffer для хранения аттрибьютов
    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Создаем EBO (element buffer object)
    GLuint ebo;
    glCreateBuffers(1, &ebo);
    glNamedBufferData(ebo, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vao, ebo);


    // Создаем vertex buffer object
    GLuint vertexBindingPoint = 0;
    GLuint vbo;
    glCreateBuffers(1, &vbo);
    glNamedBufferData(vbo, sizeof(vertArray), vertArray, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vao, vertexBindingPoint, vbo, 0, sizeof(Vertex));

    GLuint positionAttributeSlot = 0;
    glVertexArrayAttribFormat(vao, positionAttributeSlot, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribBinding(vao, positionAttributeSlot, vertexBindingPoint);
    glEnableVertexArrayAttrib(vao, positionAttributeSlot);

    GLuint colorAttributeSlot = 1;
    glVertexArrayAttribFormat(vao, colorAttributeSlot, 4, GL_FLOAT, GL_FALSE, offsetof(Vertex, color));
    glVertexArrayAttribBinding(vao, colorAttributeSlot, vertexBindingPoint);
    glEnableVertexArrayAttrib(vao, colorAttributeSlot);

    // events
    glfwSetCursorPosCallback(window->getWindow(), cursor_position_callback);
    glfwSetKeyCallback(window->getWindow(), key_callback);

    while (!window->shouldClose()) {
        glClearColor(0.1f, 0.3f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // triangle 
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        //glDrawArrays(GL_LINES, 0, 4);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window->getWindow());

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    Shader::deleteProgram(shaderProgram);

    glfwDestroyWindow(window->getWindow());
    glfwTerminate();
    return 0;
}