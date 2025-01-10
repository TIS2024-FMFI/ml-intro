#include "ImGuiApp.h"
#include <iostream>

ImGuiApp::ImGuiApp(HINSTANCE hInstance) : hInstance(hInstance), hwnd(nullptr), running(true), currentScenario(1), bias(0), learningRate(0), activationFunction(ReLu), color(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)) {}

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

    //CleanupDeviceWGL(hwnd, &g_MainWindow);
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


    //frameBuffer = new FrameBuffer(rendererSize.x, rendererSize.y);
    //renderer = new Renderer(frameBuffer, &camera);

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

    //wglMakeCurrent(nullptr, nullptr);
    //::ReleaseDC(hWnd, data->hDC);
}

void ImGuiApp::Render() {
    ImVec2 displaySize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(displaySize);

    ImGui::Begin("Main Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    RenderMenuBar();

    switch (currentScenario) {
    case 1:
        RenderScenario_1();
        break;
    case 2:
        RenderScenario_2();
        break;
    case 3:
        RenderScenario_3();
        break;

    default:
        break;
    }

    ImGui::End();    
}

void ImGuiApp::RenderMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Scenarios")) {
            if (ImGui::MenuItem("Scenario 1", nullptr, currentScenario == 1)) currentScenario = 1;
            if (ImGui::MenuItem("Scenario 2", nullptr, currentScenario == 2)) currentScenario = 2;
            if (ImGui::MenuItem("Scenario 3", nullptr, currentScenario == 3)) currentScenario = 3;
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Study Material")) {
            if (ImGui::MenuItem("Material 1")) {}
            if (ImGui::MenuItem("Material 2")) {}
            if (ImGui::MenuItem("Material 3")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void ImGuiApp::RenderScenario_1() {
    // Renderer
    RendererFrame();
    /*ImGui::BeginChild("Renderer", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    
    ImGui::EndChild();*/

    ImGui::SameLine();

    // Controls
    ImGui::BeginChild("Controls", ImVec2(0, 0));

    if (ImGui::CollapsingHeader("Input")) {
        GradientColorPicker("input", (float*)&color);
        ImGui::SameLine();
        ImGui::Text("Selected Color: ");
        ImGui::SameLine();
        ImGui::ColorButton("##Preview", color);
    }

    if (ImGui::CollapsingHeader("Bias")) {
        ImGui::SliderFloat("bias", &bias, -1.0f, 1.0f);
    }

    if (ImGui::CollapsingHeader("Learning Rate")) {
        ImGui::SliderFloat("learning rate", &learningRate, 0.0f, 1.0f);
    }

    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    if (ImGui::CollapsingHeader("Activation Function")) {
        ImGui::TreeNodeEx("ReLu", (activationFunction == ReLu) ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunction = ReLu;
        }
        ImGui::TreeNodeEx("Sigmoid", (activationFunction == Sigmoid) ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunction = Sigmoid;
        }
        ImGui::TreeNodeEx("Tanh", (activationFunction == Tanh) ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunction = Tanh;
        }
    }

    ImGui::Button("Run");
    ImGui::Button("Load");
    ImGui::Button("Save");

    ImGui::EndChild();
}

void ImGuiApp::RenderScenario_2() {
    // Renderer
    ImGui::BeginChild("Renderer", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

    ImGui::EndChild();

    ImGui::SameLine();

    // Controls
    ImGui::BeginChild("Controls", ImVec2(0, 0));

    if (ImGui::CollapsingHeader("Input")) {
        ImGui::ColorEdit3("input", (float*)&color);
    }

    if (ImGui::CollapsingHeader("Bias")) {
        ImGui::SliderFloat("bias", &bias, -1.0f, 1.0f);
    }

    if (ImGui::CollapsingHeader("Learning Rate")) {
        ImGui::SliderFloat("learning rate", &learningRate, 0.0f, 1.0f);
    }

    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    if (ImGui::CollapsingHeader("Activation Function")) {
        ImGui::TreeNodeEx("ReLu", (activationFunction == ReLu) ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunction = ReLu;
        }
        ImGui::TreeNodeEx("Sigmoid", (activationFunction == Sigmoid) ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunction = Sigmoid;
        }
        ImGui::TreeNodeEx("Tanh", (activationFunction == Tanh) ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunction = Tanh;
        }
    }

    ImGui::Button("Run");
    ImGui::Button("Load");
    ImGui::Button("Save");

    ImGui::EndChild();
}

void ImGuiApp::RenderScenario_3() {
    ImGui::Text("Scenario %d", currentScenario);
    ImGui::Columns(2);

    // Left part
    ImGui::Text("Left part of Scenario %d", currentScenario);
    ImGui::NextColumn();

    // Right part
    ImGui::Text("Right part of Scenario %d", currentScenario);

    ImGui::Columns(1);
}

void ImGuiApp::GradientColorPicker(const char* label, float* color) {
    ImVec2 gradient_size(200, 200);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
    ImVec2 gradient_end(cursor_pos.x + gradient_size.x, cursor_pos.y + gradient_size.y);

    draw_list->AddRectFilledMultiColor(
        cursor_pos, gradient_end,
        IM_COL32(0, 0, 0, 255),
        IM_COL32(255, 0, 0, 255),
        IM_COL32(255, 255, 0, 255),
        IM_COL32(0, 255, 0, 255)
    );

    // Capture mouse interaction
    ImGui::InvisibleButton("##GradientPicker", gradient_size);
    if (ImGui::IsItemActive()) {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        float x_ratio = (mouse_pos.x - cursor_pos.x) / gradient_size.x;
        float y_ratio = (mouse_pos.y - cursor_pos.y) / gradient_size.y;

        x_ratio = x_ratio < 0 ? 0.0f : (x_ratio > 1 ? 1.0f : x_ratio); //camp(x,0,1)
        y_ratio = y_ratio < 0 ? 0.0f : (y_ratio > 1 ? 1.0f : y_ratio);

        color[0] = x_ratio;      //r
        color[1] = y_ratio;      //g
        color[2] = 0.0f;         //b
    }

    // Highlight the current color position on the gradient
    float marker_x = cursor_pos.x + gradient_size.x * color[0];
    float marker_y = cursor_pos.y + gradient_size.y * color[1];
    draw_list->AddCircle(ImVec2(marker_x, marker_y), 5.0f, IM_COL32(255, 255, 255, 255), 12, 2.0f);
}


void ImGuiApp::RendererFrame() {
    ImVec2 space = ImGui::GetContentRegionAvail();
    ImGui::BeginChild("Renderer", ImVec2(space.x/2, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

    ImVec2 rendererScale = ImGui::GetContentRegionAvail();
    //frameBuffer->RescaleFrameBuffer(rendererScale.x, rendererScale.y);
    //renderer->renderScene();

    /*ImGui::Image(
        (ImTextureID)renderer->getFrameTexture(),
        rendererScale,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );*/

    ImGui::EndChild();
}

//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
//    ImGuiApp app(hInstance);
//    if (!app.Initialize())
//        return 1;
//    app.Run();
//    return 0;
//}
