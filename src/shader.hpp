#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <unordered_map>

class Shader {
public:
    Shader(const std::string& path, GLenum shaderType);
    void destroy();
    void compile();

    GLuint getId() const;

private:
    std::string readFile(const std::string& path);

    GLuint m_shaderId;
};

class ShaderProgram {
public:
    ShaderProgram(Shader vertexShader, Shader fragmentShader);
    void destroy();

    GLuint getId() const;

    void uploadVec4(const char* varName, const glm::vec4& vec4) const;
	void uploadVec3(const char* varName, const glm::vec3& vec3) const;
	void uploadVec2(const char* varName, const glm::vec2& vec2) const;
	void uploadIVec4(const char* varName, const glm::ivec4& vec4) const;
	void uploadIVec3(const char* varName, const glm::ivec3& vec3) const;
	void uploadIVec2(const char* varName, const glm::ivec2& vec2) const;
	void uploadFloat(const char* varName, float value) const;
	void uploadInt(const char* varName, int value) const;
	void uploadIntArray(const char* varName, int length, const int* array) const;
	void uploadUInt(const char* varName, uint32_t value) const;
	void uploadBool(const char* varName, bool value) const;

	void uploadMat4(const char* varName, const glm::mat4& mat4) const;
	void uploadMat3(const char* varName, const glm::mat3& mat3) const;

private:
    GLuint m_progId;
};

struct ShaderVariable {
    ShaderVariable(const std::string& n, GLint loc, GLuint prog);

    bool operator==(const ShaderVariable& other) const;

    std::string name;
    GLint varLocation;
    GLuint shaderProgram;
};