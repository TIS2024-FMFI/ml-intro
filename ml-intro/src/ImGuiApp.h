#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <glad/glad.h>
#include <Windows.h>
#include <tchar.h>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"

#include "renderer/Renderer.h"

struct WGL_WindowData { HDC hDC; };

static HGLRC g_hRC = nullptr;
static WGL_WindowData g_MainWindow = { nullptr };

class ImGuiApp {
public:
    ImGuiApp(HINSTANCE hInstance);
    ~ImGuiApp();
    bool Initialize();
    void Run();

private:
    enum ActivationFunction
    {
        ReLu,
        Sigmoid,
        Tanh
    };

    HWND hwnd;
    WNDCLASSEX wc;
    HINSTANCE hInstance;
    
    bool running;
    int currentScenario;
    ActivationFunction activationFunction;
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
    void RenderScenario_1();
    void RenderScenario_2();
    void RenderScenario_3();
    void GradientColorPicker(const char* label, float* color);
    void RendererFrame();
};

