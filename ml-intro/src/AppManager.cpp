#include "AppManager.h"


AppManager::AppManager() : gui(nullptr)
{
	testing = new NeuralNetwork(3, 2, 1, nullptr, nullptr); // No activation functions provided (default setup)

	nN1 = new NeuralNetwork(
		2,
		0,
		1,
		nullptr, // No hidden activation function
		nullptr // Sigmoid activation function for the output layer
	);

	nN2 = new NeuralNetwork(
		3,
		5,
		7,
		nullptr, // ReLU activation function for the hidden layer
		nullptr // ReLU activation function for the output layer
	);
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
	std::vector<std::vector<double>> trainingData;
	std::vector<std::vector<double>> targets;
	int currScenario = gui->getCurrentScenrio();
	switch (currScenario) {
	case 1:
		trainingData = generateTrainingSet1(10);
		targets = generateTargets1(trainingData);
		std::cout << "Input: " << trainingData << " Target: " << targets << "\n";
		std::cout << "====================================================================== " << "\n";
		std::cout << "Predictions before training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			std::vector<double> prediction = nN1->predict(trainingData[i]);
			std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << " Target: " << targets[i] << "\n";
		}
		nN1->fit(trainingData, targets, 100);
		std::cout << "Predictions After training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			std::vector<double> prediction = nN1->predict(trainingData[i]);
			std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << " Target: " << targets[i] << "\n";
		}
		break;
	case 2:
		trainingData = generateTrainingSet2(10);
		targets = generateTargets2(trainingData);
		std::cout << "Input: " << trainingData << " Target: " << targets << "\n";
		std::cout << "====================================================================== " << "\n";
		std::cout << "Predictions before training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			std::vector<double> prediction = nN2->predict(trainingData[i]);
			std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << " Target: " << targets[i] << "\n";
		}
		nN2->fit(trainingData, targets, 1000);
		std::cout << "Predictions After training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			std::vector<double> prediction = nN2->predict(trainingData[i]);
			std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << " Target: " << targets[i] << "\n";
		}
		break;
		break;

	default:
		break;
	}
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

void AppManager::setNetworkActivationFunction(std::shared_ptr<Function> activationFunctionHidden, std::shared_ptr<Function> activationFunctionOutput)
{
	std::shared_ptr<Function> tempH = activationFunctionHidden;
	std::shared_ptr<Function> tempO = activationFunctionOutput;
	if (tempO->name() == "SoftMax") {
		tempO = nullptr;
	}
	int currScenario = gui->getCurrentScenrio();

	switch (currScenario) {
	case 1:
		nN1->setHiddenActivationFunction(tempH);
		nN1->setOutputActivationFunction(tempO);
		break;
	case 2:
		nN2->setHiddenActivationFunction(tempH);
		nN2->setOutputActivationFunction(tempO);
		break;
	case 3:
		nN3->setHiddenActivationFunction(tempH);
		nN3->setOutputActivationFunction(tempO);
		break;

	default:
		break;
	}
}
