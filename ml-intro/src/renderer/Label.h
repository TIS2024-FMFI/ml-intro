#pragma once

#include <vector>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "ShaderPrograms.h"
#include <ft2build.h>
#include FT_FREETYPE_H

using namespace glm;
using namespace std;

struct Character {
    vec2 textureSize;
    vec2 textureOffset;
    vec2 bearing;
    unsigned int advance;
};

class Label {
public:
    vec3 pos;
    float weight;
    string text;

    Label(vec3 position, float weight);

    static void InitializeBuffers();
    static void AddLabel(const Label& label);
    static void UploadData();
    static void RenderLabels(Camera* cam);
    static void ClearLabels();

private:
    static GLuint VAO, VBO;
    static GLuint rectVAO, rectVBO;

    static map<char, Character> characters;
    static bool initialized;
    static GLuint textAtlas;
    static vec2 atlasSize;
    static float scale;

    static vector<float> labelData;
    static vector<float> rectData;
    static int vertexCount;
    static int rectVxCount;

    static void GenerateFontAtlas(const string& fontPath);
    static void AddRectangle(vec2 bl, vec2 tr, vec3 pos);
};
