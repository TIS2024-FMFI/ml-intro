#pragma once
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderPrograms {
public:
    ShaderPrograms(const ShaderPrograms&) = delete;
    ShaderPrograms& operator=(const ShaderPrograms&) = delete;

    static ShaderPrograms& getInstance() {
        static ShaderPrograms instance;
        return instance;
    }

    GLuint getNodeShaderProgramId() { return NodeShaderProgram; }
    GLuint getEdgeShaderProgramId() { return EdgeShaderProgram; }
    GLuint getRectShaderProgramId() { return RectShaderProgram; }
    GLuint getTextShaderProgramId() { return TextShaderProgram; }

private:
    ShaderPrograms();
    ~ShaderPrograms();

    std::string loadShaderSource(const char* filePath);
    GLuint compileShader(GLenum type, const std::string& source);
    GLuint createShaderProgram(const char* vertexPath, const char* fragmentPath);

    GLuint NodeShaderProgram;
    GLuint EdgeShaderProgram;
    GLuint RectShaderProgram;
    GLuint TextShaderProgram;
};
