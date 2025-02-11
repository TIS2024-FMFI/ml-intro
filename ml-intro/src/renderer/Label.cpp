
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
float Label::FontSize = 1;

Label::Label(vec3 position, float weight) : pos(position), weight(weight) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(3);
    stream << weight;
    text = stream.str();
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


void Label::GenerateFontAtlas(const string& fontPath) {
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR::FREETYPE: Could not initialize FreeType Library" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
        FT_Done_FreeType(ft);
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);


    // Determine atlas dimensions
    atlasSize = vec2(0, 0);
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph " << c << std::endl;
            continue;
        }
        atlasSize.x += face->glyph->bitmap.width;
        atlasSize.y = std::max(atlasSize.y, (float)face->glyph->bitmap.rows);
    }

    glGenTextures(1, &textAtlas);
    glBindTexture(GL_TEXTURE_2D, textAtlas);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasSize.x, atlasSize.y, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    int xOffset = 0;
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYPE: Failed to load Glyph " << c << std::endl;
            continue;
        }
        auto bitmap = face->glyph->bitmap;
        glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, 0, bitmap.width, bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

        Character character = {
            vec2(bitmap.width, bitmap.rows),
            vec2(xOffset, 0),
            vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters.insert(std::pair<char, Character>(c, character));

        xOffset += face->glyph->bitmap.width;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}


void Label::AddLabel(const Label& label) {
    float scale = Label::scale * FontSize;
    float x = 0;
    for (const char& c : label.text) {
        Character ch = characters[c];

        x -= (ch.advance >> 6) / 2. * scale;
    }

    vec3 pos = label.pos;
    vec2 tl = vec2(0.);
    vec2 br = vec2(0.);
    for (int i = 0; i < label.text.length(); i++) {
        char c = label.text[i];
        Character ch = characters[c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = 0 - (atlasSize.y / 2.0 + ch.textureSize.y - ch.bearing.y) * scale + 0.05;

        float w = ch.textureSize.x * scale;
        float h = ch.textureSize.y * scale;

        if (i == 0) tl = vec2(xpos, ypos);
        tl.t = std::min(tl.y, ypos);
        br.y = std::max(br.y, ypos + h);
        br.x = xpos + w;

        vec2 texOffset = ch.textureOffset / atlasSize;
        vec2 texSize = ch.textureSize / atlasSize;

        labelData.insert(labelData.end(), {
            xpos    , ypos + h, texOffset.x            , texOffset.y            , pos.x, pos.y, pos.z, label.weight,
            xpos + w, ypos    , texOffset.x + texSize.x, texOffset.y + texSize.y, pos.x, pos.y, pos.z, label.weight,
            xpos    , ypos    , texOffset.x            , texOffset.y + texSize.y, pos.x, pos.y, pos.z, label.weight,

            xpos,     ypos + h, texOffset.x            , texOffset.y            , pos.x, pos.y, pos.z, label.weight,
            xpos + w, ypos + h, texOffset.x + texSize.x, texOffset.y            , pos.x, pos.y, pos.z, label.weight,
            xpos + w, ypos    , texOffset.x + texSize.x, texOffset.y + texSize.y, pos.x, pos.y, pos.z, label.weight
            });

        vertexCount += 6;
        x += (ch.advance >> 6) * scale;
    }

    AddRectangle(tl, br, pos);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);


}


void Label::UploadData() {
    if (!initialized) return;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, labelData.size() * sizeof(float), labelData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, rectData.size() * sizeof(float), rectData.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Label::RenderLabels(Camera* cam) {
    if (!initialized) return;

    glEnable(GL_DEPTH_TEST);

    static GLuint shaderProgram = ShaderPrograms::getInstance().getTextShaderProgramId();
    glUseProgram(shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, value_ptr(cam->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, value_ptr(cam->GetProjMatrix()));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textAtlas);
    glUniform1i(glGetUniformLocation(shaderProgram, "textureAtlas"), 1);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    glBindVertexArray(0);


    static GLuint rect_shaderProgram = ShaderPrograms::getInstance().getRectShaderProgramId();
    glUseProgram(rect_shaderProgram);

    glUniformMatrix4fv(glGetUniformLocation(rect_shaderProgram, "view"), 1, GL_FALSE, value_ptr(cam->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(rect_shaderProgram, "projection"), 1, GL_FALSE, value_ptr(cam->GetProjMatrix()));

    glBindVertexArray(rectVAO);
    glDrawArrays(GL_TRIANGLES, 0, rectVxCount);
    glBindVertexArray(0);
}

void Label::ClearLabels() {
    labelData.clear();
    rectData.clear();
    vertexCount = 0;
    rectVxCount = 0;
}

void Label::AddRectangle(vec2 tl, vec2 br, vec3 pos) {
    rectData.insert(rectData.end(), {
            tl.x, br.y, pos.x, pos.y, pos.z,
            br.x, tl.y, pos.x, pos.y, pos.z,
            tl.x, tl.y, pos.x, pos.y, pos.z,

            tl.x, br.y, pos.x, pos.y, pos.z,
            br.x, br.y, pos.x, pos.y, pos.z,
            br.x, tl.y, pos.x, pos.y, pos.z
        });

    rectVxCount += 6;
}