#include "AppManager.h"

AppManager::AppManager() : gui(nullptr), neuralNetwork(nullptr)
{
	testing = new NeuralNetwork(3, 2, 1, 0.01, [](double x) { return 0; }, [](double x) { return 0; }); // Example configuration
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

// TODO
void AppManager::runNetwork()
{
}

// TODO
void AppManager::saveNetwork()
{
	testing->saveNetwork("xdddddd");
}

// TODO
void AppManager::loadNetwork()
{
}

// TODO
void AppManager::setNetworkInput()
{
}

void AppManager::setNetworkBias(float bias)
{
	neuralNetwork->setBias(bias);
}

void AppManager::setNetworkLearningRate(float learningRate)
{
	neuralNetwork->setLearningRate(learningRate);
}

void AppManager::setNetworkActivationFunction(Function activationFunction)
{
	std::function<double(double)> func = [&activationFunction](double x) { return activationFunction.function(x); };
	std::function<double(double)> deriv = [&activationFunction](double x) { return activationFunction.derivative(x); };
	neuralNetwork->setActivationFunction(func, deriv);
}
