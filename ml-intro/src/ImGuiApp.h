#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <vector>
#include <glad/glad.h>
#include <Windows.h>
#include <tchar.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "renderer/Renderer.h"
#include "functions.cpp"
#include "string"
#include "myGui/Scenario.h"
#include "myGui/ScenarioFactory.h"
#include "MyWindow.h"


class AppManager;

struct WGL_WindowData { HDC hDC; };

static HGLRC g_hRC = nullptr;
static WGL_WindowData g_MainWindow = { nullptr };

class ImGuiApp {
public:
    double bias = 1;
    double learningRate = 0.05f;
    int activationFunctionOutput = 0;
    int activationFunctionHidden = 0;

    ImGuiApp(AppManager& appManager);
    ~ImGuiApp();
    void Run();

    int getCurrentScenrio() { return currentScenario; }
    void setOuput(int Output) { output = Output; }
    int getEpochs() { return epochs; }
    std::vector<float> getInput() { return networkInputVector; };

    Renderer* renderer;

private:
    MyWindow myWindow = MyWindow(1280, 720);
    HWND hwnd;
    WNDCLASSEX wc;
    HINSTANCE hInstance;


    AppManager* appManager;
    bool darkTheme = false;
    bool isRenderDebugOpen = false;
    int currentScenario = 1;

    std::vector<float> networkInputVector = { 0, 0 };
    int output = -1;
    int epochs = 10;
    std::string activationFunctionNameHidden = "Relu";
    vector<string> outputLayerFunctOptions = { "ReLu", "Sigmoid", "Tanh", "SoftMax" };
    vector<string> hiddenLayerFunctOptions = { "ReLu", "Sigmoid", "Tanh" };

    ImVec2 rendererSize = ImVec2(500, 500);
    Camera camera = Camera(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0), &rendererSize.x, &rendererSize.y);
    FrameBuffer* frameBuffer = nullptr;

    static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
    static void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
    void InitializeNewFrame();
    void myMenuBar();
    void myControlPanelFrame();
    void myRendererFrame();
    void myRendererDebugPanel();
    void MouseCameraHandeler();

};

