#include "ImGuiApp.h"
#include <iostream>
#include "AppManager.h"
#include "openWindow.h"
#include "myGui/CustomLightStyle.h"


ImGuiApp::ImGuiApp(AppManager& appManager) : appManager(&appManager)
{
    const char* glsl_version = "#version 330";
    ImGui::CreateContext();
    darkTheme ? ImGui::StyleColorsDark() : SetSoftLightTheme();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding.x = 3;
    style.WindowPadding.y = 3;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(myWindow.window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    frameBuffer = new FrameBuffer(rendererSize.x, rendererSize.y);
    renderer = &Renderer::getInstance();
    renderer->Init(frameBuffer, &camera);
}

ImGuiApp::~ImGuiApp() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiApp::Run() {
    appManager->predictCurrentGuiInput();
    appManager->renderNewScene();
    while (!glfwWindowShouldClose(myWindow.window)) {
        InitializeNewFrame();

        myRendererFrame();

        myControlPanelFrame();

        if (isRenderDebugOpen) {
            myRendererDebugPanel();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(myWindow.window);
        glfwPollEvents();
    }
}

void ImGuiApp::InitializeNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create the docking environment
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;


    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::GetStyle().WindowRounding = 0.0f;

    if (ImGui::Begin("InvisibleWindowDock", nullptr, windowFlags)) {

        myMenuBar();

        ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDock");
        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        static auto first_time = true;
        if (first_time)
        {
            first_time = false;

            ImGui::DockBuilderRemoveNode(dockSpaceId); // clear any previous layout
            ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoUndocking);
            ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetIO().DisplaySize);

            ImGuiID left_dock_id;
            ImGuiID right_dock_id;
            ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, 0.2f, &right_dock_id, &left_dock_id);

            ImGui::DockBuilderDockWindow("Network Control Panel", right_dock_id);
            ImGui::DockBuilderDockWindow("Renderer", left_dock_id);
            ImGui::DockBuilderFinish(dockSpaceId);
        }
    }ImGui::End();
}

void ImGuiApp::myMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Scenarios")) {
            if (ImGui::MenuItem("Scenario 1", nullptr, currentScenario == 1)) {
                currentScenario = 1; 
                networkInputVector = { networkInputVector[0], networkInputVector[1] };
                renderer->PlaneRender(); appManager->renderNewScene();
            }
            if (ImGui::MenuItem("Scenario 2", nullptr, currentScenario == 2)) { 
                currentScenario = 2;
                networkInputVector = { networkInputVector[0], networkInputVector[1], 0 };
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
            if (ImGui::MenuItem("Material 1")) { openWindow("resources/scenario1.html"); }
            if (ImGui::MenuItem("Material 2")) { openWindow("resources/scenario2.html"); }
            if (ImGui::MenuItem("Material 3")) { openWindow("resources/scenario3.html"); }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Settings")) {
            if (ImGui::MenuItem("Render Text", nullptr, renderer->isEnabled())) { 
                renderer->setText(!renderer->isEnabled()); 
                renderer->renderLabels(); 
            };
            if (ImGui::MenuItem("Renderer debug", nullptr, &isRenderDebugOpen));
            if (ImGui::MenuItem(darkTheme ? "Light Theme" : "Dark Theme")) {
                darkTheme ? SetSoftLightTheme() : ImGui::StyleColorsDark();
                darkTheme = !darkTheme;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void ImGuiApp::myRendererFrame() {
    ImGuiWindowClass window_class1;
    window_class1.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
    window_class1.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
    window_class1.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoTabBar;

    static ImVec2 lastRendererScale = ImVec2(0, 0);
    ImGui::SetNextWindowClass(&window_class1);
    if (ImGui::Begin("Renderer", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar)) {

        MouseCameraHandeler();
        rendererSize = ImGui::GetContentRegionAvail();
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

    }ImGui::End();
}

void ImGuiApp::myControlPanelFrame() {
    ImGui::Begin("Network Control Panel", NULL);

    std::unique_ptr<Scenario> scenario = ScenarioFactory::create(currentScenario);

    if (scenario) {
        if (scenario->customInputBlock("Input", networkInputVector)) {
            appManager->predictCurrentGuiInput();
            appManager->updateCurrentScene();
        }
        scenario->customOutputBlock("Output", output);
        int out = scenario->customPickOutputBlock("Pick Output");
        if (out != -1) appManager->tellOutput(out); // one day you will be nicer
        if (scenario->customDoubleSlider("Bias", bias, -1, 1)) {
            appManager->updateCurrentScene();
        }
        scenario->customDoubleSlider("Learning Rate", learningRate, 0, 0.5);
        scenario->customMultiSelector("Activation Function (output layer)", outputLayerFunctOptions, activationFunctionOutput);
        if (currentScenario != 1) {
            scenario->customMultiSelector("Activation Function (hidden layer)", hiddenLayerFunctOptions, activationFunctionHidden);
        }
        scenario->customIntSlider("epochs", epochs, 1, 100);
    }

    
    if (ImGui::Button("Run")) {
        appManager->runNetwork();
        appManager->updateCurrentScene();
    }
    ImGui::SameLine();
    if (ImGui::Button("Save NN")) {
        appManager->saveNetwork();
    }
    ImGui::SameLine();
    if (ImGui::Button("Load NN")) {
        appManager->loadNetwork();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        appManager->resetNetworkToRandomValues();
        appManager->predictCurrentGuiInput();
        appManager->updateCurrentScene();
    }
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("sets all the weights to random values");
    }
    if(currentScenario !=3){
        if (ImGui::Button("Save TrainingSet")) {
            appManager->saveTrainingSet();
        }
        ImGui::SameLine();
        if (ImGui::Button("Load TrainingSet")) {
            appManager->loadTrainingSet();
        }
    }

    ImGui::End();
}

void ImGuiApp::MouseCameraHandeler()
{
    static bool isDragging = false;
    static POINT dragStartPos;

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
            camera.ProcessMouseDelta(vec2(mousePos.x - dragStartPos.x, mousePos.y - dragStartPos.y) * .05f);
            SetCursorPos(dragStartPos.x, dragStartPos.y);
        }
    }
    else if (isDragging)
    {
        SetCursorPos(dragStartPos.x, dragStartPos.y);
        isDragging = false;
        ShowCursor(TRUE);
    }

    if (isDragging || ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
        camera.ProcessMouseScroll(ImGui::GetIO().MouseWheel);
    }
    if (ImGui::IsKeyDown(ImGuiKey_W)) camera.ProcessKeyboardInput(vec2(0, 1));
    if (ImGui::IsKeyDown(ImGuiKey_A)) camera.ProcessKeyboardInput(vec2(-1,0));
    if (ImGui::IsKeyDown(ImGuiKey_S)) camera.ProcessKeyboardInput(vec2(0,-1));
    if (ImGui::IsKeyDown(ImGuiKey_D)) camera.ProcessKeyboardInput(vec2(1, 0));
}

void ImGuiApp::myRendererDebugPanel() {
    if(ImGui::Begin("Renderer Debug Panel", &isRenderDebugOpen)) {
        if (ImGui::CollapsingHeader("Camera Settings")) {
            ImGui::Checkbox("Camera Free Cam", &camera.isFreeCam);
            ImGui::SliderFloat("Sensitivity", &camera.sensitivity, 0, 5);
            ImGui::SliderFloat("Scroll Speed", &camera.scrSpeed, 0, 5);
            ImGui::SliderFloat("Camera Speed", &camera.camSpeed, 0, 1);
        }
        if (ImGui::CollapsingHeader("Renderer Settings")) {
            ImGui::ColorEdit3("Background Color", &frameBuffer->bcg.x);
            ImGui::ColorEdit3("Negative Color", &renderer->negative.x);
            ImGui::ColorEdit3("Positive Color", &renderer->positive.x);
            ImGui::SliderFloat("Vertex Size", &Node::PointSize, 0, 10);
            ImGui::SliderFloat("Edge Size", &Edge::LineSize, 0, 10);
            if (ImGui::SliderFloat("FontSize", &Label::FontSize, 0, 4)) {
                Label::ClearLabels();
                renderer->renderLabels();
            }
            if (ImGui::Checkbox("Square Render", &renderer->squareRender)) {
                appManager->renderNewScene();
            }
            if (ImGui::SliderFloat("Node Spacing", &renderer->nodeSpacing, 0, 10) ||
                ImGui::SliderFloat("Layer Spacing", &renderer->layerSpacing, 0, 10)) 
            {
                appManager->renderNewScene();
            }
        }
    }ImGui::End();
}