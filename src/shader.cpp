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
        return 0;
    }

    return someShader;
}

GLuint Shader::createShaderProgram(const std::string& vert_shader_path, const std::string& frag_shader_path) {
    std::string vertexShaderSource = readFile(vert_shader_path);
    std::string fragmentShaderSource = readFile(frag_shader_path);

    GLuint vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    if (vertexShader && fragmentShader == 0) {
        return 0;
    } 

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint isLinked = 0;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return 0;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLint numUniforms;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &numUniforms);

    GLint maxCharLength;
    glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxCharLength);

    if (numUniforms > 0 && maxCharLength > 0) {
        char* buffer = new char[sizeof(char) * maxCharLength];
        for (size_t i = 0; i < numUniforms; ++i) {
            int length, size;
            GLenum dataType;

            glGetActiveUniform(shaderProgram, i, maxCharLength, &length, &size, &dataType, buffer);
            GLint varLocation = glGetUniformLocation(shaderProgram, buffer);

            std::cout << "Uniform ";
            for (size_t i = 0; i < maxCharLength; ++i) {
                std::cout << buffer[i];
            }
            std::cout << " has location " << varLocation << std::endl;
            
            std::fill(buffer, buffer + maxCharLength, '\0');
        }
        delete[] buffer;
    }
    
    return shaderProgram;
}

void Shader::deleteProgram(GLuint someProgram) {
    if (someProgram) {
        glDeleteProgram(someProgram);
    }
}