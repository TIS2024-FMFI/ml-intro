#include "AppManager.h"

AppManager::AppManager() : gui(nullptr), neuralNetwork(nullptr)
{

}

AppManager::~AppManager()
{
	delete gui;
	delete neuralNetwork;
}

void AppManager::run()
{
	if (gui == nullptr) {
		HINSTANCE hInstance = GetModuleHandle(nullptr);

		gui = new ImGuiApp(*this, hInstance);
		if (!gui->Initialize()) {
			MessageBox(nullptr, _T("Failed to initialize ImGui application"), _T("Error"), MB_OK | MB_ICONERROR);
			return;
		}
	}

	gui->Run();
}
