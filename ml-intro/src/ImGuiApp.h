#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "imgui_impl_opengl3.h"
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <tchar.h>
#include "functions.cpp"

class AppManager; // Forward declaration of AppManager

struct WGL_WindowData { HDC hDC; };

static HGLRC g_hRC = nullptr;
static WGL_WindowData g_MainWindow = { nullptr };

class ImGuiApp {
public:
    ImGuiApp(AppManager& parent, HINSTANCE hInstance);
    ~ImGuiApp();
    bool Initialize();
    void Run();

    int getCurrentScenrio() { return currentScenario; }
    Function* getActivationFunction();
    float getBias() { return bias; }
    float getLearningRate() { return learningRate; }
    ImVec4 getInput() { return color; }


private:
    enum ActivationFunction
    {
        relu,
        sigmoid,
        tanh
    };

    HWND hwnd;
    WNDCLASSEX wc;
    HINSTANCE hInstance;
    
    AppManager* parent;
    bool running;
    int currentScenario;
    ActivationFunction activationFunction;
    float bias;
    float learningRate;
    ImVec4 color;

    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
    static void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
    void Render();
    void RenderMenuBar();
    void RenderScenario_1();
    void RenderScenario_2();
    void RenderScenario_3();
    void GradientColorPicker(const char* label, float* color);
};

