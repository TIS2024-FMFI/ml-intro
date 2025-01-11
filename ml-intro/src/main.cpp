#include <glad/glad.h>
#include <Windows.h>
#include <tchar.h>
#include "ImGuiApp.h"
#include "openWindow.h"
#include "Perceptron.h"

int main(void)
{
    HINSTANCE hInstance = GetModuleHandle(nullptr);

    ImGuiApp app(hInstance);
    if (!app.Initialize()) {
        MessageBox(nullptr, _T("Failed to initialize ImGui application"), _T("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }

    app.Run();

    return 0;
}

//#include <ft2build.h>
//#include FT_FREETYPE_H
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <iostream>
//#include <map>
//#include <chrono>
//#include <string>
//#include <vector>
//
//// Vertex Shader Source
//const char* vertexShaderSource = R"(
//#version 330 core
//layout (location = 0) in vec2 aPos;
//layout (location = 1) in vec2 aTexCoord;
//
//out vec2 TexCoord;
//
//uniform mat4 projection;
//
//void main() {
//    gl_Position = projection * vec4(aPos, 0.0, 1.0);
//    TexCoord = aTexCoord;
//}
//)";
//
//// Fragment Shader Source
//const char* fragmentShaderSource = R"(
//#version 330 core
//in vec2 TexCoord;
//out vec4 FragColor;
//
//uniform sampler2D textAtlas;
//
//void main() {
//    float sampled = texture(textAtlas, TexCoord).r; // Assuming GL_RED format
//    FragColor = vec4(sampled, sampled, sampled, 1.0); // Display grayscale
//}
//)";
//
//// Helper function to compile shaders
//unsigned int CompileShader(unsigned int type, const char* source) {
//    unsigned int shader = glCreateShader(type);
//    glShaderSource(shader, 1, &source, nullptr);
//    glCompileShader(shader);
//
//    int success;
//    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
//        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
//    }
//    return shader;
//}
//
//// Helper function to link shader program
//unsigned int CreateShaderProgram(const char* vertexSource, const char* fragmentSource) {
//    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
//    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
//
//    unsigned int shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    int success;
//    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//    if (!success) {
//        char infoLog[512];
//        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
//        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//    }
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    return shaderProgram;
//}
//
//class TextureAtlas {
//public:
//    struct Character {
//        glm::ivec2 Size;
//        glm::ivec2 Bearing;
//        unsigned int Advance;
//        glm::vec2 TexOffset;
//        glm::vec2 TexSize;
//    };
//
//    TextureAtlas(const std::string& fontPath, unsigned int fontSize){
//
//        if (FT_Init_FreeType(&m_Library)) {
//            std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//            exit(1);
//        }
//
//        if (FT_New_Face(m_Library, fontPath.c_str(), 0, &m_Face)) {
//            std::cerr << "ERROR::FREETYPE: Failed to load font" << std::endl;
//            exit(1);
//        }
//
//        FT_Set_Pixel_Sizes(m_Face, 0, fontSize);
//
//
//        for (unsigned char c = 0; c < 128; c++) {
//            if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER)) {
//                std::cerr << "WARNING: Failed to load glyph '" << c << "'\n";
//                continue;
//            }
//
//            m_AtlasWidth += m_Face->glyph->bitmap.width;
//            m_AtlasHeight = std::max(m_AtlasHeight, m_Face->glyph->bitmap.rows);
//        }
//
//        glGenTextures(1, &m_AtlasTexture);
//        glBindTexture(GL_TEXTURE_2D, m_AtlasTexture);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_AtlasWidth, m_AtlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//
//        LoadCharacters();
//    }
//
//    ~TextureAtlas() {
//        glDeleteTextures(1, &m_AtlasTexture);
//        FT_Done_Face(m_Face);
//        FT_Done_FreeType(m_Library);
//    }
//
//    const std::map<char, Character>& GetCharacters() const { return m_Characters; }
//    unsigned int GetAtlasTexture() const { return m_AtlasTexture; }
//
//private:
//    FT_Library m_Library;
//    FT_Face m_Face;
//    unsigned int m_AtlasTexture;
//    unsigned int m_AtlasWidth, m_AtlasHeight;
//    std::map<char, Character> m_Characters;
//
//    void LoadCharacters() {
//        unsigned int xOffset = 0;
//
//        for (unsigned char c = 0; c < 128; ++c) {
//            if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER)) {
//                std::cerr << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
//                continue;
//            }
//
//            auto bitmap = m_Face->glyph->bitmap;
//            glm::vec2 bScale = { bitmap.width, bitmap.rows };
//            
//
//            glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, 0, bitmap.width, bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);
//
//            Character character = {
//                glm::ivec2(bitmap.width, bitmap.rows),
//                glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
//                static_cast<unsigned int>(m_Face->glyph->advance.x >> 6),
//                glm::vec2(xOffset / static_cast<float>(m_AtlasWidth), 0),
//                glm::vec2(m_Face->glyph->bitmap.width / static_cast<float>(m_AtlasWidth), m_Face->glyph->bitmap.rows / static_cast<float>(m_AtlasHeight))
//            };
//
//            m_Characters.insert(std::make_pair(c, character));
//
//            xOffset += bitmap.width;
//        }
//    }
//};
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//void processInput(GLFWwindow* window) {
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//}
//
//int main() {
//    // Initialize GLFW
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return -1;
//    }
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    GLFWwindow* window = glfwCreateWindow(1980, 50, "Texture Atlas Display", nullptr, nullptr);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//        std::cerr << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    // Load the texture atlas
//    TextureAtlas atlas("resources/fonts/arial.ttf", 48);
//    GLuint atlasTexture = atlas.GetAtlasTexture();
//
//    // Set up a simple quad to display the texture atlas
//    float vertices[] = {
//        // positions       // texture coords
//        -1.0f,  1.0f,  0.0f, 1.0f,
//        -1.0f, -1.0f,  0.0f, 0.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//
//        -1.0f,  1.0f,  0.0f, 1.0f,
//         1.0f, -1.0f,  1.0f, 0.0f,
//         1.0f,  1.0f,  1.0f, 1.0f
//    };
//
//    GLuint VAO, VBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
//
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
//
//    // Simple shader setup (replace with your own shader code if needed)
//    const char* vertexShaderSource = R"(
//        #version 330 core
//        layout (location = 0) in vec2 aPos;
//        layout (location = 1) in vec2 aTexCoord;
//
//        out vec2 TexCoord;
//
//        void main() {
//            TexCoord = aTexCoord;
//            gl_Position = vec4(aPos, 0.0, 1.0);
//        }
//    )";
//
//    const char* fragmentShaderSource = R"(
//        #version 330 core
//        out vec4 FragColor;
//
//        in vec2 TexCoord;
//
//        uniform sampler2D atlasTexture;
//
//        void main() {
//            FragColor = texture(atlasTexture, TexCoord);
//        }
//    )";
//
//    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
//    glCompileShader(vertexShader);
//
//    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
//    glCompileShader(fragmentShader);
//
//    GLuint shaderProgram = glCreateProgram();
//    glAttachShader(shaderProgram, vertexShader);
//    glAttachShader(shaderProgram, fragmentShader);
//    glLinkProgram(shaderProgram);
//
//    glDeleteShader(vertexShader);
//    glDeleteShader(fragmentShader);
//
//    glUseProgram(shaderProgram);
//    glUniform1i(glGetUniformLocation(shaderProgram, "atlasTexture"), 0);
//
//    while (!glfwWindowShouldClose(window)) {
//        processInput(window);
//
//        glClear(GL_COLOR_BUFFER_BIT);
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, atlasTexture);
//
//        glBindVertexArray(VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteProgram(shaderProgram);
//
//    glfwTerminate();
//    return 0;
//}