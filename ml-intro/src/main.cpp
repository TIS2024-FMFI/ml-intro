#include <Windows.h>
#include <tchar.h>
#include "ImGuiApp.h"

int blackWindow() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

int main(void)
{
    //blackWindow();

    //renderImGuiDemo();

    HINSTANCE hInstance = GetModuleHandle(nullptr);

    ImGuiApp app(hInstance);
    if (!app.Initialize()) {
        MessageBox(nullptr, _T("Failed to initialize ImGui application"), _T("Error"), MB_OK | MB_ICONERROR);
        return 1;
    }

    app.Run();

    return 0;
}