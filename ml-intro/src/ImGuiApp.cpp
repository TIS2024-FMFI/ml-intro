#include "ImGuiApp.h"
#include <iostream>
#include "AppManager.h"

ImGuiApp::ImGuiApp(AppManager& appManager, HINSTANCE hInstance) : appManager(&appManager), hInstance(hInstance), hwnd(nullptr), running(true),
currentScenario(1), bias(1.0), learningRate(0.05), color(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), bitmap(canvasSize, std::vector<bool>(canvasSize, false)) {}

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

std::shared_ptr<Function> ImGuiApp::getActivationFunctionOutput()
{
    if (activationFunctionNameOutput == "ReLu") {
        return std::make_shared<ReLu>();
    }
    else if (activationFunctionNameOutput == "Sigmoid") {
        return std::make_shared<Sigmoid>();
    }
    else if (activationFunctionNameOutput == "Tanh") {
        return std::make_shared<Tanh>();
    }
    else if (activationFunctionNameOutput == "SoftMax") {
        return std::make_shared<SoftMax>();
    }
    else {
        return nullptr; // Default activation function
    }
}

std::shared_ptr<Function> ImGuiApp::getActivationFunctionHidden()
{
    if (activationFunctionNameHidden == "ReLu") {
        return std::make_shared<ReLu>();

    }
    else if (activationFunctionNameHidden == "Sigmoid") {
        return std::make_shared<Sigmoid>();

    }
    else if (activationFunctionNameHidden == "Tanh") {
        return std::make_shared<Tanh>();

    }
    
    else {
        return nullptr; // No activation function
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
            if (ImGui::MenuItem("Scenario 1", nullptr, currentScenario == 1)) { currentScenario = 1; Renderer::getInstance().PlaneRender();appManager->renderNewScene(); }
            if (ImGui::MenuItem("Scenario 2", nullptr, currentScenario == 2)) { currentScenario = 2; Renderer::getInstance().PlaneRender(); appManager->renderNewScene(); }
            if (ImGui::MenuItem("Scenario 3", nullptr, currentScenario == 3)) { currentScenario = 3; Renderer::getInstance().SquareRender(); appManager->renderNewScene(); }
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
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

bool ImGuiApp::CustomButton(const char* label, ImVec4 color) {
    ImGui::PushID(label);

    ImVec2 button_size = ImVec2(60, 30);

    ImVec2 p = ImGui::GetCursorScreenPos();
    bool is_clicked = ImGui::InvisibleButton(label, button_size);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    ImU32 color_bg = ImGui::GetColorU32(color);
    ImU32 color_text = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

    draw_list->AddRectFilled(p, ImVec2(p.x + button_size.x, p.y + button_size.y), color_bg);

    ImVec2 text_size = ImGui::CalcTextSize(label);
    ImVec2 text_pos = ImVec2(p.x + (button_size.x - text_size.x) / 2, p.y + (button_size.y - text_size.y) / 2);
    draw_list->AddText(text_pos, color_text, label);

    ImGui::PopID();

    return is_clicked;
}

void ImGuiApp::SendParameters() {
    appManager->setNetworkBias(bias);
    appManager->setNetworkLearningRate(learningRate);
    appManager->setNetworkActivationFunction(getActivationFunctionHidden(), getActivationFunctionOutput());
}
void ImGuiApp::RenderRunButton() {
    if (ImGui::Button("Run")) {
        SendParameters();

        appManager->runNetwork();
        appManager->updateCurrentScene();
    }
}

void ImGuiApp::RenderSaveButton()
{
    if (ImGui::Button("Save")) {
        appManager->saveNetwork();
    }
}

void ImGuiApp::RenderLoadButton()
{
    if (ImGui::Button("Load")) {
        appManager->loadNetwork();
    }
}

void ImGuiApp::RenderActivationFunctionsOuput()
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    if (ImGui::CollapsingHeader("Activation Function (output layer)")) {
        ImGui::TreeNodeEx("ReLu ##output", (activationFunctionNameOutput == "ReLu") ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunctionNameOutput = "ReLu";
        }
        ImGui::TreeNodeEx("Sigmoid ##output", (activationFunctionNameOutput == "Sigmoid") ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunctionNameOutput = "Sigmoid";
        }
        ImGui::TreeNodeEx("Tanh ##output", (activationFunctionNameOutput == "Tanh") ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunctionNameOutput = "Tanh";
        }
        ImGui::TreeNodeEx("SoftMax ##output", (activationFunctionNameOutput == "SoftMax") ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunctionNameOutput = "SoftMax";
        }
        
    }
}

void ImGuiApp::RenderActivationFunctionsHidden()
{
    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    if (ImGui::CollapsingHeader("Activation Function (hidden layer)")) {
        ImGui::TreeNodeEx("ReLu ##hidden", (activationFunctionNameHidden == "ReLu") ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunctionNameHidden = "ReLu";
        }
        ImGui::TreeNodeEx("Sigmoid ##hidden", (activationFunctionNameHidden == "Sigmoid") ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunctionNameHidden = "Sigmoid";
        }
        ImGui::TreeNodeEx("Tanh ##hidden", (activationFunctionNameHidden == "Tanh") ? node_flags | ImGuiTreeNodeFlags_Selected : node_flags);
        if (ImGui::IsItemClicked()) {
            activationFunctionNameHidden = "Tanh";
        }
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

    if (ImGui::CollapsingHeader("Output")) {
        RenderOuput_1();
    }

    if (ImGui::CollapsingHeader("Tell Output")) {
        if (CustomButton("Red", ImVec4(1.0f, 0.0f, 0.0f, 1.0f))){
            SendParameters();
            appManager->tellOutput(0);
            
        }

        ImGui::SameLine();

        if (CustomButton("Green", ImVec4(0.0f, 1.0f, 0.0f, 1.0f))) {
            SendParameters();
            appManager->tellOutput(1);
            
        }
    }

    

    if (ImGui::CollapsingHeader("Bias")) {
        ImGui::SliderFloat("bias", &bias, -1.0f, 1.0f);
    }

    if (ImGui::CollapsingHeader("Learning Rate")) {
        ImGui::SliderFloat("learning rate", &learningRate, 0.0f, 1.0f);
    }

    RenderActivationFunctionsOuput();
    RenderActivationFunctionsHidden();
    if (ImGui::CollapsingHeader("Epochs")) {
        ImGui::SliderInt("epochs", &epochs, 10, 100);
    }
    RenderRunButton();
    RenderLoadButton();
    RenderSaveButton();

    ImGui::EndChild();
}

void ImGuiApp::RenderScenario_2() {
    // Renderer
    RendererFrame();


    ImGui::SameLine();

    // Controls
    ImGui::BeginChild("Controls", ImVec2(0, 0));

    if (ImGui::CollapsingHeader("Input")) {
        ImGui::ColorEdit3("input", (float*)&color);
    }

    if (ImGui::CollapsingHeader("Output")) {
        RenderOuput_2();
    }

    if (ImGui::CollapsingHeader("Tell Output")) {
        RenderTellOuput_2();
    }

    

    if (ImGui::CollapsingHeader("Bias")) {
        ImGui::SliderFloat("bias", &bias, -1.0f, 1.0f);
    }

    if (ImGui::CollapsingHeader("Learning Rate")) {
        ImGui::SliderFloat("learning rate", &learningRate, 0.0f, 1.0f);
    }

    RenderActivationFunctionsOuput();
    RenderActivationFunctionsHidden();

    if (ImGui::CollapsingHeader("Epochs")) {
        ImGui::SliderInt("epochs", &epochs, 10, 100);
    }

    RenderRunButton();
    RenderLoadButton();
    RenderSaveButton();

    ImGui::EndChild();
}

void ImGuiApp::RenderScenario_3() {
    // Renderer
    RendererFrame();



    ImGui::SameLine();

    // Controls
    ImGui::BeginChild("Controls", ImVec2(0, 0));

    if (ImGui::CollapsingHeader("Input")) {
        DrawBitmapEditor();
    }

    if (ImGui::CollapsingHeader("Output")) {
        if (output != -1) {
            ImGui::Text("Result: %d", output);
        } else {
            ImGui::Text("No result yet");
        }

        //for (size_t i = 0; i < 10; i++)
        //{
        //    //ImGui::Button(std::to_string(i).c_str());
        //    ImGui::Text("%d", i);

        //    if (i < 9) {
        //        ImGui::SameLine();
        //    }
        //}
    }

    if (ImGui::CollapsingHeader("Tell Output")) {
        for (size_t i = 0; i < 10; i++)
        {
            if (ImGui::Button(std::to_string(i).c_str())) {
                SendParameters();
                appManager->tellOutput(i);
                
            }

            if (i < 9) {
                ImGui::SameLine();
            }
        }
    }    

    if (ImGui::CollapsingHeader("Bias")) {
        ImGui::SliderFloat("bias", &bias, -1.0f, 1.0f);
    }

    if (ImGui::CollapsingHeader("Learning Rate")) {
        learningRate = clamp(learningRate, 0.0f, 0.025f);
        ImGui::SliderFloat("learning rate", &learningRate, 0.0f, 0.025f);
    }

    RenderActivationFunctionsOuput();
    RenderActivationFunctionsHidden();
    if (ImGui::CollapsingHeader("Epochs")) {
        ImGui::SliderInt("epochs", &epochs, 10, 50);
    }
    RenderRunButton();
    RenderLoadButton();
    RenderSaveButton();

    ImGui::EndChild();
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
    ImGui::BeginChild("Renderer", ImVec2(space.x / 2, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

    ImVec2 rendererScale = ImGui::GetContentRegionAvail();
    MouseDeltaHandeler();
    frameBuffer->RescaleFrameBuffer(rendererScale.x, rendererScale.y);
    renderer->renderScene();

    ImGui::Image(
        (ImTextureID)renderer->getFrameTexture(),
        rendererScale,
        ImVec2(0, 1),
        ImVec2(1, 0)
    );

    ImGui::EndChild();
}

void ImGuiApp::DrawBitmapEditor() {
    // Reserve space for the canvas
    ImVec2 canvasSizeInPixels = ImVec2(canvasSize * pixelSize, canvasSize * pixelSize);
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // Draw the canvas
    for (int y = 0; y < canvasSize; y++) {
        for (int x = 0; x < canvasSize; x++) {
            ImVec2 topLeft = ImVec2(canvasPos.x + x * pixelSize, canvasPos.y + y * pixelSize);
            ImVec2 bottomRight = ImVec2(topLeft.x + pixelSize, topLeft.y + pixelSize);

            // Draw the pixel
            if (bitmap[y][x]) {
                drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(0, 0, 0, 255)); // Black
            }
            else {
                drawList->AddRectFilled(topLeft, bottomRight, IM_COL32(255, 255, 255, 255)); // White
            }

            // Draw the grid
            drawList->AddRect(topLeft, bottomRight, IM_COL32(200, 200, 200, 255));
        }
    }

    // Handle mouse interaction
    if (ImGui::IsMouseHoveringRect(canvasPos, ImVec2(canvasPos.x + canvasSizeInPixels.x, canvasPos.y + canvasSizeInPixels.y))) {
        if (ImGui::IsMouseDown(0)) { // Left mouse button
            ImVec2 mousePos = ImGui::GetMousePos();
            int x = (mousePos.x - canvasPos.x) / pixelSize;
            int y = (mousePos.y - canvasPos.y) / pixelSize;

            if (x >= 0 && x < canvasSize && y >= 0 && y < canvasSize) {
                bitmap[y][x] = true; // Set pixel "on"
            }
        }
        if (ImGui::IsMouseDown(1)) { // Right mouse button
            ImVec2 mousePos = ImGui::GetMousePos();
            int x = (mousePos.x - canvasPos.x) / pixelSize;
            int y = (mousePos.y - canvasPos.y) / pixelSize;

            if (x >= 0 && x < canvasSize && y >= 0 && y < canvasSize) {
                bitmap[y][x] = false; // Set pixel "off"
            }
        }
    }

    // Reserve space for the canvas so that other ImGui widgets are positioned below it
    ImGui::Dummy(canvasSizeInPixels);

    if (ImGui::Button("Clear")) {
        for (int y = 0; y < canvasSize; y++) {
            for (int x = 0; x < canvasSize; x++) {
                bitmap[y][x] = false;
            }
        }
    }
}

void ImGuiApp::RenderOuput_1() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 p = ImGui::GetCursorScreenPos();

    float x = p.x + 4.0f;
    float y = p.y + 4.0f;
    float sz = 40.0f;
    const float spacing = 10.0f;

    ImVec4 red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    std::vector<ImVec4> colors = { red, green };

    for (size_t i = 0; i < colors.size(); ++i) {
        ImVec4 color = colors[i];

        if (i == output) {
            ImVec4 darker = ImVec4(color.x * 0.7f, color.y * 0.7f, color.z * 0.7f, 1.0f);
            draw_list->AddCircle(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.6f, ImColor(darker), 0, 3.0f);
        }

        draw_list->AddCircleFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, ImColor(color));
        x += sz + spacing;
    }


    ImGui::Dummy(ImVec2((sz + spacing), (sz + spacing)));
}

void ImGuiApp::RenderOuput_2() {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 p = ImGui::GetCursorScreenPos();

    float x = p.x + 4.0f;
    float y = p.y + 4.0f;
    float sz = 40.0f;
    const float spacing = 10.0f;

    ImVec4 red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    ImVec4 magenta = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
    ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 blue = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    ImVec4 cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
    ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    std::vector<ImVec4> colors = { red, magenta, yellow, white, blue, cyan, green };

    for (size_t i = 0; i < colors.size(); ++i) {
        ImVec4 color = colors[i];

        if (i == output) {
            ImVec4 darker = ImVec4(color.x * 0.7f, color.y * 0.7f, color.z * 0.7f, 1.0f);
            draw_list->AddCircle(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.6f, ImColor(darker), 0, 3.0f);
        }

        draw_list->AddCircleFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz * 0.5f, ImColor(color));
        x += sz + spacing;
    }


    ImGui::Dummy(ImVec2((sz + spacing), (sz + spacing)));
}

void ImGuiApp::RenderTellOuput_2() {
    ImVec4 red = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    if (CustomButton("Red", red)) {
        SendParameters();
        appManager->tellOutput(0);
    }
    ImGui::SameLine();

    ImVec4 magenta = ImVec4(1.0f, 0.0f, 1.0f, 1.0f);
    if (CustomButton("Magenta", magenta)) {
        SendParameters();
        appManager->tellOutput(1);
    }
    ImGui::SameLine();

    ImVec4 yellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    if (CustomButton("Yellow", yellow)) {
        SendParameters();
        appManager->tellOutput(2);
        
    }
    ImGui::SameLine();

    ImVec4 white = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    if (CustomButton("White", white)) {
        SendParameters();
        appManager->tellOutput(3);
        
    }
    ImGui::SameLine();

    ImVec4 blue = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    if (CustomButton("Blue", blue)) {
        SendParameters();
        appManager->tellOutput(4);
    }
    ImGui::SameLine();

    ImVec4 cyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
    if (CustomButton("Cyan", cyan)) {
        SendParameters();
        appManager->tellOutput(5);
    }
    ImGui::SameLine();

    ImVec4 green = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    if (CustomButton("Green", green)) {
        SendParameters();
        appManager->tellOutput(6);
        
    }
}

void ImGuiApp::MouseDeltaHandeler()
{
    static bool isDragging = false;
    static POINT dragStartPos;

    ImVec2 framePos = ImGui::GetCursorScreenPos();
    ImVec2 frameSize = ImGui::GetContentRegionAvail();
    ImVec2 bottomLeft = ImVec2(framePos.x + frameSize.x, framePos.y + frameSize.y);
    ImVec2 mid = ImVec2(250, 250);

    POINT mousePos;
    if (!GetCursorPos(&mousePos)) {
        mousePos.x = mousePos.y = 0;
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        if (!isDragging && !ImGui::IsAnyItemActive() &&
            ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) 
        {
            ShowCursor(FALSE);
            dragStartPos = mousePos;
            isDragging = true;
        }
        else if (isDragging) {
            camera.MoveCamera(vec2(mousePos.x - dragStartPos.x, mousePos.y - dragStartPos.y) * .05f);

            SetCursorPos(dragStartPos.x, dragStartPos.y);
        }
    }
    else if (isDragging)
    {
        SetCursorPos(dragStartPos.x, dragStartPos.y);
        isDragging = false;
        ShowCursor(TRUE);
    }

    //mouseScrollHandeler
    if (isDragging || ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
        camera.ProcessMouseScroll(ImGui::GetIO().MouseWheel);
    }
}

//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
//    ImGuiApp app(hInstance);
//    if (!app.Initialize())
//        return 1;
//    app.Run();
//    return 0;
//}


