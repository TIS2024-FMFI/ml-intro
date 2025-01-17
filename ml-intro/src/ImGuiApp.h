#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <vector>
#include <glad/glad.h>
#include <Windows.h>
#include <tchar.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include "renderer/Renderer.h"
#include "functions.cpp"
#include "string"


class AppManager;

struct WGL_WindowData { HDC hDC; };

static HGLRC g_hRC = nullptr;
static WGL_WindowData g_MainWindow = { nullptr };

class ImGuiApp {
public:
    ImGuiApp(AppManager& appManager, HINSTANCE hInstance);
    ~ImGuiApp();
    bool Initialize();
    void Run();

    int getCurrentScenrio() { return currentScenario; }
    std::shared_ptr<Function> getActivationFunctionOutput();
    std::shared_ptr<Function> getActivationFunctionHidden();
    float getBias() { return bias; }
    float getLearningRate() { return learningRate; }
    ImVec4 getInput() { return color; }

private:
    HWND hwnd;
    WNDCLASSEX wc;
    HINSTANCE hInstance;

    const int canvasSize = 28;
    const float pixelSize = 20.0f;
    std::vector<std::vector<bool>> bitmap;

    AppManager* appManager;
    bool running;
    int currentScenario;
    std::string activationFunctionNameOutput;
    std::string activationFunctionNameHidden;
    float bias;
    float learningRate;
    ImVec4 color;

    ImVec2 rendererSize = ImVec2(500, 500);
    Camera camera = Camera(glm::vec3(5, 0, 0), glm::vec3(0, 0, 0), &rendererSize.x, &rendererSize.y);
    FrameBuffer* frameBuffer = nullptr;
    Renderer* renderer = nullptr;

    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
    static void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
    void Render();
    void RenderMenuBar();
    bool CustomButton(const char* label, ImVec4 color);
    void RenderRunButton();
    void RenderSaveButton();
    void RenderLoadButton();
    void RenderActivationFunctionsOuput();
    void RenderActivationFunctionsHidden();
    void RenderScenario_1();
    void RenderScenario_2();
    void RenderScenario_3();
    void GradientColorPicker(const char* label, float* color);
    void RendererFrame();
    void DrawBitmapEditor();
    void RenderOuput_1();
    void RenderOuput_2();
    void RenderTellOuput_2();
};

