#include "AppManager.h"
#include <iomanip>

AppManager::AppManager() : gui(nullptr)
{

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
	std::vector<Eigen::VectorXd> trainingData;
	std::vector<Eigen::VectorXd> targets;
	int currScenario = gui->getCurrentScenrio();

	switch (currScenario) {
	case 1:
		trainingData = generateTrainingSet1(20);
		targets = generateTargets1(trainingData);

		std::cout << "======================================================================\n";
		std::cout << "Predictions before training:\n";

		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN1->predict(trainingData[i]);
			double normalizedPrediction = prediction[0];
			if (nN1->getActivationFuncName() == "Tanh") {
				normalizedPrediction = (prediction[0] + 1.0) / 2.0; // Pre Tanh normalizácia na [0, 1]
			}
			std::cout << std::fixed << std::setprecision(3);
			std::cout << "Input: " << trainingData[i].transpose() << " Prediction: " << prediction.transpose() << "NORMALIZED: " << normalizedPrediction
				<< " Target: " << targets[i].transpose() << "\n";
		}

		nN1->fit(trainingData, targets, 100);

		std::cout << "Predictions after training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN1->predict(trainingData[i]);
			double normalizedPrediction = prediction[0];
			if (nN1->getActivationFuncName() == "Tanh") {
				normalizedPrediction = (prediction[0] + 1.0) / 2.0; // Pre Tanh normalizácia na [0, 1]
			}
			std::cout << std::fixed << std::setprecision(3);
			std::cout << "Input: " << trainingData[i].transpose() << " Prediction: " << prediction.transpose() << " NORMALIZED: " << normalizedPrediction
				<< std::setprecision(0) <<  " Target: " << targets[i].transpose() << "\n";
		}
		break;

	case 2:
		trainingData = generateTrainingSet2(20);
		targets = generateTargets2(trainingData);

		std::cout << "======================================================================\n";
		std::cout << "Predictions before training:\n";

		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN2->predict(trainingData[i]);

			std::cout << std::fixed << std::setprecision(3);
			std::cout << "Input: " << trainingData[i].transpose() << " Prediction: " << prediction.transpose()
				<< std::setprecision(0)  << " Target: " << targets[i].transpose() << "\n";
		}

		nN2->fit(trainingData, targets, 100);

		std::cout << "Predictions after training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN2->predict(trainingData[i]);
			std::cout << std::fixed << std::setprecision(3);
			std::cout << "Input: " << trainingData[i].transpose() << " Prediction: " << prediction.transpose()
				<< std::setprecision(0) <<  " Target: " << targets[i].transpose() << "\n";
		}
		break;

	default:
		break;
	}
}

// TODO
void AppManager::saveNetwork()
{
	int currScenario = gui->getCurrentScenrio();


	switch (currScenario) {
	case 1: {
		nN1->saveNetwork("xd1");
		break;
	}

	case 2: {
		nN2->saveNetwork("xd2");
		break;
	}

	default:
		nN3->saveNetwork("xd3");
		break;
	}
}

// TODO
void AppManager::loadNetwork()
{
	int currScenario = gui->getCurrentScenrio();


	switch (currScenario) {
	case 1: {
		nN1->loadNetwork(openFileDialog());
		break;
	}

	case 2: {
		nN2->loadNetwork(openFileDialog());
		break;
	}

	default:
		nN3->loadNetwork(openFileDialog());
		break;
	}
}

// TODO
Eigen::VectorXd AppManager::setNetworkInput()
{
	int currScenario = gui->getCurrentScenrio();
	Eigen::VectorXd inputs;

	switch (currScenario) {
	case 1: {
		// Create a vector of size 2 for scenario 1
		inputs = Eigen::VectorXd(2);
		inputs(0) = gui->getInput().x;
		inputs(1) = gui->getInput().y;
		break;
	}

	case 2: {
		// Create a vector of size 3 for scenario 2
		inputs = Eigen::VectorXd(3);
		inputs(0) = gui->getInput().x;
		inputs(1) = gui->getInput().y;
		inputs(2) = gui->getInput().z;
		break;
	}

	default:
		// Return an empty vector in case of an unsupported scenario
		inputs = Eigen::VectorXd(0);
		break;
	}

	return inputs;
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

int AppManager::tellOutput(int output)
{
	Eigen::VectorXd inputs = setNetworkInput();
	int currScenario = gui->getCurrentScenrio();
	int result = 0;

	switch (currScenario) {
	case 1: {
		double outputValue = static_cast<double>(output);
		Eigen::VectorXd prediction = nN1->predict(inputs);
		nN1->train(inputs, Eigen::VectorXd::Constant(1, outputValue));
		double normalizedPrediction = prediction[0];
		if (nN1->getActivationFuncName() == "Tanh") {
			normalizedPrediction = (prediction[0] + 1.0) / 2.0; // Pre Tanh normalizácia na [0, 1]
		}
		result = static_cast<int>(std::round(normalizedPrediction));
		if (result > 1) {
			result = 1;
		}
		std::cout << std::fixed << std::setprecision(3);
		std::cout << "INPUTS: " << inputs.transpose() << "\n";
		std::cout << "PREDICTION: " << prediction.transpose() << "\n";
		std::cout << std::setprecision(0) <<  "RESULT: " << result << " TOLD: " << output << "\n";
		break;
	}

	case 2: {
		Eigen::VectorXd outputVector = Eigen::VectorXd::Zero(7);
		if (output >= 0 && output < 7) {
			outputVector[output] = 1.0;
		}
		Eigen::VectorXd prediction = nN2->predict(inputs);
		nN2->train(inputs, outputVector);

		result = static_cast<int>(std::distance(prediction.data(), std::max_element(prediction.data(), prediction.data() + 7)));

		std::cout << std::fixed << std::setprecision(3);
		std::cout << "INPUTS: " << inputs.transpose() << "\n";
		std::cout << "PREDICTION: " << prediction.transpose() << "\n";
		std::cout << std::setprecision(0) << "RESULT: " << result << " TOLD: " << output << "\n";
		break;
	}

	default:
		break;
	}
	return result;
}
