#include "AppManager.h"


AppManager::AppManager() : gui(nullptr)
{
	testing = new NeuralNetwork(3, 2, 1, [](double x) { return 0; }, [](double x) { return 0; }); // Example configuration
	nN1 = new NeuralNetwork(2, 0, 1,nullptr, nullptr, [](double x) { return Sigmoid().function(x); }, [](double x) { return Sigmoid().derivative(x); });
	nN2 = new NeuralNetwork(3, 5, 7,[](double x) { return ReLu().function(x); }, [](double x) { return ReLu().derivative(x); });
}

AppManager::~AppManager()
{
	delete gui;
	delete nN1;
	delete nN2;
	delete nN3;
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
	testing->saveNetwork("saving3.json");
}

// TODO
void AppManager::loadNetwork()
{
	testing->loadNetwork(openFileDialog());
}

// TODO
void AppManager::setNetworkInput()
{
	int currScenario = gui->getCurrentScenrio();
	std::vector<double> inputs;
	switch (currScenario) {
	case 1:
		inputs.push_back(gui->getInput().x);
		inputs.push_back(gui->getInput().y);
		break;
	case 2:
		inputs.push_back(gui->getInput().x);
		inputs.push_back(gui->getInput().y);
		inputs.push_back(gui->getInput().z);
		break;

	default:
		break;
	}
}

void AppManager::setNetworkBias(float bias)
{
	int currScenario = gui->getCurrentScenrio();
	
	switch (currScenario) {
	case 1:
		nN1->setBias(bias);
		break;
	case 2:
		nN2->setBias(bias);
		break;
	case 3:
		nN3->setBias(bias);
		break;

	default:
		break;
	}
}

void AppManager::setNetworkLearningRate(float learningRate)
{
	int currScenario = gui->getCurrentScenrio();

	switch (currScenario) {
	case 1:
		nN1->setLearningRate(learningRate);
		break;
	case 2:
		nN2->setLearningRate(learningRate);
		break;
	case 3:
		nN3->setLearningRate(learningRate);
		break;

	default:
		break;
	}
	
}

void AppManager::setNetworkActivationFunction(Function activationFunction)
{
	std::function<double(double)> func = [&activationFunction](double x) { return activationFunction.function(x); };
	std::function<double(double)> deriv = [&activationFunction](double x) { return activationFunction.derivative(x); };
	
	int currScenario = gui->getCurrentScenrio();
	switch (currScenario) {
	case 1:
		nN1->setHiddenActivationFunction(func, deriv);
		break;
	case 2:
		nN2->setHiddenActivationFunction(func, deriv);
		break;
	case 3:
		nN3->setHiddenActivationFunction(func, deriv);
		break;

	default:
		break;
	}
}
