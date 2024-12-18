#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <map>
#include "Camera.h"
#include <iomanip>
#include <sstream>

using namespace std;
using namespace glm;

struct Character {
    unsigned int TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    unsigned int Advance;
};

class Label {
public:
    Label(Camera* camera);
    void drawLabel(vec3 pos, float weight);

private:
    const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec4 vertex; // (position.xy, texCoords.xy)
    out vec2 TexCoords;

    uniform mat4 mvp;

    void main() {
        gl_Position = mvp * vec4(vertex.xy, 0.0, 1.0);
        TexCoords = vertex.zw;
    }
    )";

    const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoords;
    out vec4 FragColor;

    uniform sampler2D text;
    uniform float weight;

    void main() {
        vec2 tmpUV0 = vec2(TexCoords.x, 1.0-TexCoords.y);
        vec2 tmpUV1 = (tmpUV0 - .5)*1.2 + .5;
        vec2 tmpUV2 = (tmpUV0 - .5)*1.4 + .5;

        vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, tmpUV1).r);
        vec3 col = vec3(clamp(-weight, 0., 1.), clamp(weight, 0., 1.), .0);
        
        float out1 = texture(text, tmpUV0).r;
        float out2 = texture(text, tmpUV2).r;
        float outline = clamp(clamp(out1 + out2, 0., 1.) - sampled.w, 0., 1. );
        if (sampled.w + outline == 0.) discard;
        FragColor = (vec4(col, 1.) * sampled.w) + vec4(0., 0., 0., outline);
    }
    )";


    unsigned int VAO, VBO;
    GLuint shaderProgram;
    Camera* camera;

    map<char, Character> characters;
    vec3 col = { 1.0, 0.6, 0.8 };
    float scale = 0.04;
};