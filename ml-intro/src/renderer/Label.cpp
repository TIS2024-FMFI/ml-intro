
#include "Label.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <ft2build.h>
#include FT_FREETYPE_H


GLuint Label::VAO = 0;
GLuint Label::VBO = 0;
GLuint Label::rectVAO = 0;
GLuint Label::rectVBO = 0;

GLuint Label::textAtlas = 0;
map<char, Character> Label::characters;
bool Label::initialized = false;
vector<float> Label::labelData;
vector<float> Label::rectData;
int Label::vertexCount = 0;
int Label::rectVxCount = 0;
vec2 Label::atlasSize = vec2(0, 0);
float Label::scale = 0.002f;


Label::Label(vec3 position, float weight) : pos(position), weight(weight) {
    InitializeBuffers();

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(3);
    stream << weight;
    text = stream.str();

    AddLabel(*this);
}



void Label::InitializeBuffers() {
    if (initialized) return;

    GenerateFontAtlas("resources/fonts/arial.ttf");

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); //vec2 texture Pos
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float))); //vec2 uv
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float))); //vec3 model Pos
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(7 * sizeof(float))); //float for color (activation / weight)
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    initialized = true;

    ////rectangle behind the text
    glGenVertexArrays(1, &rectVAO);
    glBindVertexArray(rectVAO);

    glGenBuffers(1, &rectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //vec2 texture Pos
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))); //vec3 model Pos
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}