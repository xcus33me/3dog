#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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


// upload functions

void ShaderProgram::uploadVec4(const char* varName, const glm::vec4& vec4) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform4f(varLocation, vec4.x, vec4.y, vec4.z, vec4.w);
    } 
}

void ShaderProgram::uploadVec3(const char* varName, const glm::vec3& vec3) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform3f(varLocation, vec3.x, vec3.y, vec3.z);
    }
}

void ShaderProgram::uploadVec2(const char* varName, const glm::vec2& vec2) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform2f(varLocation, vec2.x, vec2.y);
    }
}

void ShaderProgram::uploadIVec4(const char* varName, const glm::ivec4& vec4) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform4i(varLocation, vec4.x, vec4.y, vec4.z, vec4.w);
    }
}

void ShaderProgram::uploadIVec3(const char* varName, const glm::ivec3& vec3) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform3i(varLocation, vec3.x, vec3.y, vec3.z);
    }
}

void ShaderProgram::uploadIVec2(const char* varName, const glm::ivec2& vec2) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform2i(varLocation, vec2.x, vec2.y);
    }
}

void ShaderProgram::uploadFloat(const char* varName, float value) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform1f(varLocation, value);
    }
}

void ShaderProgram::uploadInt(const char* varName, int value) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform1i(varLocation, value);
    }
}

void ShaderProgram::uploadIntArray(const char* varName, int length, const int* array) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform1iv(varLocation, length, array);
    }
}

void ShaderProgram::uploadUInt(const char* varName, uint32_t value) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform1ui(varLocation, value);
    }
}

void ShaderProgram::uploadBool(const char* varName, bool value) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniform1i(varLocation, value ? 1 : 0);
    }
}

void ShaderProgram::uploadMat4(const char* varName, const glm::mat4& mat4) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniformMatrix4fv(varLocation, 1, GL_FALSE, glm::value_ptr(mat4));
    }
}

void ShaderProgram::uploadMat3(const char* varName, const glm::mat3& mat3) const {
    GLint varLocation = getShaderVariableLocation(*this, varName);
    if (varLocation != -1) {
        glUniformMatrix3fv(varLocation, 1, GL_FALSE, glm::value_ptr(mat3));
    }
}

GLint ShaderProgram::getShaderVariableLocation(const ShaderProgram& shaderProgram, const char* varName) {
    ShaderVariable var(varName, 0, shaderProgram.getId());

    auto iter = allShaderVariableLocations.find(var);
    if (iter != allShaderVariableLocations.end()) {
        return iter->varLocation;
    }
    
    std::cout << "Failed to find variable location: " << varName << std::endl;
    return -1;
}