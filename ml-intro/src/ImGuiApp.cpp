#include "ImGuiApp.h"
#include <iostream>
#include "AppManager.h"

ImGuiApp::ImGuiApp(AppManager& appManager, HINSTANCE hInstance) : appManager(&appManager), hInstance(hInstance), hwnd(nullptr) {}

ImGuiApp::~ImGuiApp() {
    if (ImGui::GetCurrentContext()) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    if (g_hRC) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(g_hRC);
    }

    if (g_MainWindow.hDC) {
        ReleaseDC(hwnd, g_MainWindow.hDC);
    }

    if (hwnd && IsWindow(hwnd)) {
        DestroyWindow(hwnd);
    }

    if (wc.lpszClassName) {
        UnregisterClass(wc.lpszClassName, wc.hInstance); // Unregister class after destroying the window
    }

    CleanupDeviceWGL(hwnd, &g_MainWindow);
}

bool ImGuiApp::Initialize() {
    wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    RegisterClassExW(&wc);
    hwnd = CreateWindowW(wc.lpszClassName, L"ML for Students", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    if (!hwnd) {
        MessageBox(nullptr, L"Failed to create window", L"Error", MB_OK | MB_ICONERROR);
        return false;
    }


    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        DestroyWindow(hwnd);
        UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return false;
    }

    wglMakeCurrent(g_MainWindow.hDC, g_hRC);

    if (!glfwInit()) {
        return false;
    }

    // initialize GLAD
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);


    frameBuffer = new FrameBuffer(rendererSize.x, rendererSize.y);
    renderer = &Renderer::getInstance();
    renderer->Init(frameBuffer, &camera);

    appManager->renderNewScene();
    return true;
}

void ImGuiApp::Run() {
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (running) {
        while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        Render();

        ImGui::Render();
        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        glViewport(0, 0, (int)displaySize.x, (int)displaySize.y);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SwapBuffers(GetDC(hwnd));
    }
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI ImGuiApp::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg) {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
            glViewport(0, 0, (int)LOWORD(lParam), (int)HIWORD(lParam));
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool ImGuiApp::CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);

    return true;
}

void ImGuiApp::CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    if (g_hRC) {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(g_hRC);
        g_hRC = nullptr;
    }

    if (data->hDC) {
        ReleaseDC(hWnd, data->hDC);
        data->hDC = nullptr;
    }
}

void ImGuiApp::Render() {
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(displaySize);

    ImGui::Begin("Main Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    RenderMenuBar();

    myRendererFrame();

    ImGui::SameLine();

    myControllPanelFrame();

    ImGui::End();
}

void ImGuiApp::RenderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Scenarios")) {
            if (ImGui::MenuItem("Scenario 1", nullptr, currentScenario == 1)) {
                currentScenario = 1; 
                networkInputVector = { networkInputVector[0], networkInputVector[1] };
                renderer->PlaneRender(); appManager->renderNewScene();
            }
            if (ImGui::MenuItem("Scenario 2", nullptr, currentScenario == 2)) { 
                currentScenario = 2;
                networkInputVector = { networkInputVector[0], networkInputVector[1], networkInputVector[2] };
                renderer->PlaneRender(); appManager->renderNewScene();
            }
            if (ImGui::MenuItem("Scenario 3", nullptr, currentScenario == 3)) { 
                currentScenario = 3; 
                networkInputVector = vector<float>(28*28, 0);
                renderer->SquareRender(); appManager->renderNewScene();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Study Material")) {
            if (ImGui::MenuItem("Material 1")) {}
            if (ImGui::MenuItem("Material 2")) {}
            if (ImGui::MenuItem("Material 3")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Render Text", nullptr, renderer->isEnabled())) { 
                renderer->setText(!renderer->isEnabled()); 
                renderer->renderLabels(); 
            };
            if (renderer->isSquareRender()) {
                if (ImGui::MenuItem("set Plane Render")) {
                    renderer->PlaneRender();
                    appManager->renderNewScene();
                }
            }
            else {
                if (ImGui::MenuItem("set Square Render")) {
                    renderer->SquareRender();
                    appManager->renderNewScene();
                }
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void ImGuiApp::myRendererFrame() {
    static ImVec2 lastRendererScale = ImVec2(0, 0);
    ImVec2 space = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("Renderer", ImVec2(space.x / 2, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

    rendererSize = ImGui::GetContentRegionAvail();
    MouseCameraHandeler();
    if (rendererSize.x != lastRendererScale.x || rendererSize.y != lastRendererScale.y) {
        frameBuffer->RescaleFrameBuffer(rendererSize.x, rendererSize.y);
        lastRendererScale = rendererSize;
    }
    glViewport(0, 0, rendererSize.x, rendererSize.y);
    renderer->renderScene();

    ImGui::Image(
        (ImTextureID)renderer->getFrameTexture(),
        rendererSize,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::EndChild();
}

void ImGuiApp::myControllPanelFrame() {
    ImGui::BeginChild("Controls", ImVec2(0, 0));

    std::unique_ptr<Scenario> scenario = ScenarioFactory::create(currentScenario);

    if (scenario) {
        if (scenario->customInputBlock("Input", networkInputVector)) {
            appManager->predictCurrentGuiInput();
            appManager->updateCurrentScene();
        }
        scenario->customOutputBlock("Output", output);
        int out = scenario->customPickOutputBlock("Pick Output");
        if (out != -1) appManager->tellOutput(out); // one day you will be nicer
        scenario->customDoubleSlider("Bias", bias, -1, 1);
        scenario->customDoubleSlider("Learning Rate", learningRate, 0, 0.5);
        scenario->customMultiSelector("Activation Function (output layer)", outputLayerFunctOptions, activationFunctionOutput);
        scenario->customMultiSelector("Activation Function (hidden layer)", hiddenLayerFunctOptions, activationFunctionHidden);
        scenario->customIntSlider("epochs", epochs, 10, 100);
    }

    
    if (ImGui::Button("Run")) {
        appManager->runNetwork();
        appManager->updateCurrentScene();
    }
    if (ImGui::Button("Save")) {
        appManager->saveNetwork();
    }
    if (ImGui::Button("Load")) {
        appManager->loadNetwork();
    }

    ImGui::EndChild();
}

void ImGuiApp::MouseCameraHandeler()
{
    ImGuiIO& io = ImGui::GetIO();
    static bool isDragging = false;

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        if (!isDragging && ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            isDragging = true;
        }
        else if (isDragging)
        {
            camera.MoveCamera(vec2(io.MouseDelta.x, io.MouseDelta.y) * 0.05f);
        }
    }
    else if (isDragging)
    {
        isDragging = false;
    }

    if (isDragging || ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
        camera.ProcessMouseScroll(io.MouseWheel);
    }
}