#include <glad/glad.h>

#include "shader.hpp"

#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>


struct hashShaderVar {
    std::size_t operator()(const ShaderVariable& key) const {
        std::size_t seed = std::hash<std::string>()(key.name);
        seed ^= std::hash<uint32_t>()(key.shaderProgram) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    } 
};

static std::unordered_set<ShaderVariable, hashShaderVar> allShaderVariableLocations;

Shader::Shader(const std::string& path, GLenum shaderType) {
    std::string shaderSrc = readFile(path);
    if (shaderSrc.empty()) {
        destroy();
    }

    const char* src = shaderSrc.c_str();
    m_shaderId = glCreateShader(shaderType);
    glShaderSource(m_shaderId, 1, &src, nullptr);
}

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

void Shader::compile() {
    glCompileShader(m_shaderId);
    
    GLint isCompiled = 0;
    glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &isCompiled);
    
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(m_shaderId, maxLength, &maxLength, infoLog.data());

        std::cout << "Shader Compilation failed: \n";
        for (GLchar c : infoLog) {
            std::cout << c;
        }

        destroy();
        return;
    }
}

void Shader::destroy() {
    if (m_shaderId) {
        glDeleteShader(m_shaderId);
    }
}

GLuint Shader::getId() const {
    return m_shaderId;
}

ShaderProgram::ShaderProgram(Shader vertexShader, Shader fragmentShader) {
    m_progId = glCreateProgram();
    glAttachShader(m_progId, vertexShader.getId());
    glAttachShader(m_progId, fragmentShader.getId());
    glLinkProgram(m_progId);

    GLint isLinked = 0;
    glGetProgramiv(m_progId, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(m_progId, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_progId, maxLength, &maxLength, &infoLog[0]);

        glDeleteProgram(m_progId);
    }

    glDetachShader(m_progId, vertexShader.getId());
    glDetachShader(m_progId, fragmentShader.getId());

    GLint numUniforms;
    glGetProgramiv(m_progId, GL_ACTIVE_UNIFORMS, &numUniforms);

    GLint maxCharLength;
    glGetProgramiv(m_progId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxCharLength);

    if (numUniforms > 0 && maxCharLength > 0) {
        char* buffer = new char[sizeof(char) * maxCharLength];
        for (size_t i = 0; i < numUniforms; ++i) {
            int length, size;
            GLenum dataType;

            glGetActiveUniform(m_progId, i, maxCharLength, &length, &size, &dataType, buffer);
            GLint varLocation = glGetUniformLocation(m_progId, buffer);

            ShaderVariable shaderVar(std::string((buffer), length), varLocation, m_progId);
            allShaderVariableLocations.emplace(shaderVar);

            std::cout << "Uniform ";
            for (size_t i = 0; i < maxCharLength; ++i) {
                std::cout << buffer[i];
            }
            std::cout << " has location " << varLocation << std::endl;

            std::fill(buffer, buffer + maxCharLength, '\0');
        }
        delete[] buffer;
    }
}

void ShaderProgram::destroy() {
    if (m_progId) {
        glDeleteProgram(m_progId);
    }
}

ShaderVariable::ShaderVariable(const std::string& n, GLint loc, GLuint prog)
    : name(n)
    , varLocation(loc)
    , shaderProgram(prog) {
}

bool ShaderVariable::operator==(const ShaderVariable& other) const {
    return (shaderProgram == other.shaderProgram && name == other.name);
}

GLuint ShaderProgram::getId() const {
    return m_progId;
}