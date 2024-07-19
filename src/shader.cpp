#include <glad/glad.h>

#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

std::string Shader::readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << path << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
}

GLuint Shader::compileShader(const std::string& shaderSrc, GLenum shaderType) {
    const char* src = shaderSrc.c_str();
    
    GLuint someShader = glCreateShader(shaderType);
    glShaderSource(someShader, 1, &src, nullptr);
    glCompileShader(someShader);

    GLint isCompiled = 0;
    glGetShaderiv(someShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(someShader, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(someShader, maxLength, &maxLength, infoLog.data());

        std::cout << "Shader Compilation failed: \n";
        for (GLchar c : infoLog) {
            std::cout << c;
        }

        glDeleteShader(someShader);
    }

    return someShader;
}

GLuint Shader::createShaderProgram(const std::string& vert_shader_path, const std::string& frag_shader_path) {
    std::string vertexShaderSource = readFile(vert_shader_path);
    std::string fragmentShaderSource = readFile(frag_shader_path);

    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    return shaderProgram;
}

void Shader::deleteProgram(GLuint someProgram) {
    if (someProgram) {
        glDeleteProgram(someProgram);
    }
}