#include <Windows.h>
#include <tchar.h>
#include "ImGuiApp.h"

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