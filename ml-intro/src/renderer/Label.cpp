
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