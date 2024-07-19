#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>

class Shader {
public:
    static GLuint createShaderProgram(const std::string& vert_shader_path, const std::string& frag_shader_path);
    static void deleteProgram(GLuint someProgram);

private:
    static std::string readFile(const std::string& path);
    static GLuint compileShader(const std::string& shaderSrc, GLenum shaderType);

};