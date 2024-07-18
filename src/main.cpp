#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Math
#include <glm/glm.hpp>

// STL
#include <iostream>

// shader
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec4 aColor;\n"
"void main()\n"
"{\n"
"   fColor = aColor;\n"
"};\n";
const char* fragmentShaderSource = "#version 330 core\n"
"in vec4 fColor;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = fColor;\n"
"}\n\0";

const int windowWidth = 1280;
const int windowHeight = 720;
const char* winodwTitle = "3dog";

// callbacks

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    std::cout << "Cursor: " << xpos << " " << ypos << std::endl;
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

void UploadVerticesAndSpecs(GLuint& vao_id, GLuint& vbo_id) {
    Vertex vertArray[3] = {
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.9f, 0.8f, 0.2f, 1.0f) },
        { glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(0.2f, 0.9f, 0.8f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.9f, 0.2f, 0.8f, 1.0f) }
    };

    // Создаем vertex array buffer для хранения аттрибьютов
    glCreateVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);

    // Создаем vertex buffer object
    glCreateBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertArray), vertArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);

    // Биндим к 0, чтобы не изменить наши буфферы
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int main() {
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW!";
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
    };

    GLFWwindow* window = glfwCreateWindow(
        windowWidth,
        windowHeight,
        winodwTitle,
        nullptr,
        nullptr
    );

    if (window == NULL) {
        std::cout << "Failed to create window!\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD!";
        return -1;
    }

    glViewport(0, 0, 1280, 720);

    // Создаем объект вертекс шейдера и получаем его ссылку.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);   
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Создаем VAO и VBO, загружаем vertices
    GLuint VaoId, VboId;
    UploadVerticesAndSpecs(VaoId, VboId);

    // events
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.8f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // triangle 
        glUseProgram(shaderProgram);
        glBindVertexArray(VaoId);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VaoId);
    glDeleteBuffers(1, &VboId);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}