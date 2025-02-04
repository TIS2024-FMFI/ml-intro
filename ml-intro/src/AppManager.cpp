#include "AppManager.h"
#include <iomanip>

AppManager::AppManager() : gui(nullptr)
{

	nN1 = new NeuralNetwork(
		2,
		0,
		1,
		nullptr,
		std::make_shared<Sigmoid>()
	);

	nN2 = new NeuralNetwork(
		3,
		5,
		7,
		std::make_shared<ReLu>(),
		nullptr
	);

	nN3 = new NeuralNetwork(
		28 * 28,
		128,
		10,
		std::make_shared<ReLu>(),
		nullptr
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
		trainingData = generateTrainingSet1(50);
		targets = generateTargets1(trainingData);

		std::cout << "======================================================================\n";
		std::cout << "Predictions before training:\n";

		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN1->predict(trainingData[i]);
			double normalizedPrediction = prediction[0];
			if (nN1->getActivationFuncName() == "Tanh") {
				normalizedPrediction = (prediction[0] + 1.0) / 2.0;
			}

			std::cout << std::fixed << std::setprecision(3);
			std::cout << "Input: " << trainingData[i].transpose() << " Prediction: " << prediction.transpose() << "NORMALIZED: " << normalizedPrediction
				<< " Target: " << targets[i].transpose() << "\n";
		}

		nN1->fit(trainingData, targets, 100);
		//sendDataToRenderer(trainingData.back());
		std::cout << "Predictions after training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN1->predict(trainingData[i]);
			double normalizedPrediction = prediction[0];
			if (nN1->getActivationFuncName() == "Tanh") {
				normalizedPrediction = (prediction[0] + 1.0) / 2.0;
			}

			std::cout << std::fixed << std::setprecision(3);
			std::cout << "Input: " << trainingData[i].transpose() << " Prediction: " << prediction.transpose() << " NORMALIZED: " << normalizedPrediction
				<< std::setprecision(0) << " Target: " << targets[i].transpose() << "\n";
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
				<< std::setprecision(0) << " Target: " << targets[i].transpose() << "\n";
		}

		nN2->fit(trainingData, targets, 100);
		//sendDataToRenderer(trainingData.back());
		std::cout << "Predictions after training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN2->predict(trainingData[i]);
			std::cout << std::fixed << std::setprecision(3);
			std::cout << "Input: " << trainingData[i].transpose() << " Prediction: " << prediction.transpose()
				<< std::setprecision(0) << " Target: " << targets[i].transpose() << "\n";
		}
		break;
	case 3: {
		std::pair<std::vector<Eigen::VectorXd>, std::vector<Eigen::VectorXd>> data = loadFixedTrainingSet();
		trainingData = data.first;
		targets = data.second;
		/*printVectorSet(trainingData);
		printVectorSet(targets);*/
		std::cout << "======================================================================\n";
		std::cout << "Predictions before training:\n";

		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN3->predict(trainingData[i]);

			std::cout << std::fixed << std::setprecision(3);
			std::cout << " Prediction: " << prediction.transpose()
				<< std::setprecision(0) << " Target: " << targets[i].transpose() << "\n";
		}
		nN3->fit(trainingData, targets, 30);
		std::cout << "Predictions after training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN3->predict(trainingData[i]);
			std::cout << std::fixed << std::setprecision(3);
			std::cout << " Prediction: " << prediction.transpose()
				<< std::setprecision(0) << " Target: " << targets[i].transpose() << "\n";
		}
		break;

	}
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

	case 3: {
		std::vector<std::vector<bool>> bitmap = gui->getBitmap();

		inputs = Eigen::VectorXd(28 * 28);

		int index = 0;
		for (const auto& row : bitmap) {
			for (bool cell : row) {
				inputs(index++) = cell ? 1.0 : 0.0;
			}
		}
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
	
	int currScenario = gui->getCurrentScenrio();

	switch (currScenario) {
	case 1:
		if (tempH != nullptr) {
			nN1->setHiddenActivationFunction(tempH);
		}
		if (tempO != nullptr) {
			if (tempO->name() == "SoftMax") {
				tempO = nullptr;
			}
			nN1->setOutputActivationFunction(tempO);
		}
		
		break;
	case 2:
		if (tempH != nullptr) {
			nN2->setHiddenActivationFunction(tempH);
		}
		if (tempO != nullptr) {
			if (tempO->name() == "SoftMax") {
				tempO = nullptr;
			}
			nN2->setOutputActivationFunction(tempO);
		}
		break;
	case 3:
		if (tempH != nullptr) {
			nN3->setHiddenActivationFunction(tempH);
		}
		if (tempO != nullptr) {
			if (tempO->name() == "SoftMax") {
				tempO = nullptr;
			}
			nN3->setOutputActivationFunction(tempO);
		}
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

		nN1->train(inputs, Eigen::VectorXd::Constant(1, outputValue));
		Eigen::VectorXd prediction = nN1->predict(inputs);
		double normalizedPrediction = prediction[0];
		if (nN1->getActivationFuncName() == "Tanh") {
			normalizedPrediction = (prediction[0] + 1.0) / 2.0;
		}
		result = static_cast<int>(std::round(normalizedPrediction));
		if (result > 1) {
			result = 1;
		}
		break;
	}

	case 2: {
		Eigen::VectorXd outputVector = Eigen::VectorXd::Zero(7);
		if (output >= 0 && output < 7) {
			outputVector[output] = 1.0;
		}

		nN2->train(inputs, outputVector);
		Eigen::VectorXd prediction = nN2->predict(inputs);
		result = static_cast<int>(std::distance(prediction.data(), std::max_element(prediction.data(), prediction.data() + 7)));
		break;
	}
	case 3: {
		Eigen::VectorXd outputVector = Eigen::VectorXd::Zero(10);
		if (output >= 0 && output < 10) {
			outputVector[output] = 1.0;
		}

		nN3->train(inputs, outputVector);
		Eigen::VectorXd prediction = nN3->predict(inputs);
		result = static_cast<int>(std::distance(prediction.data(), std::max_element(prediction.data(), prediction.data() + 10)));

		break;
	}

	default:
		break;
	}
	updateCurrentScene();
	return result;
}

std::pair<std::vector<Eigen::MatrixXd>, std::vector<Eigen::MatrixXd>> AppManager::getNetworkData() {
	int currScenario = gui->getCurrentScenrio();

	switch (currScenario) {
	case 1:
		return nN1->extractNetworkData();
	case 2:
		return nN2->extractNetworkData();
	case 3:
		return nN3->extractNetworkData();
	}
	return nN1->extractNetworkData();
}


void AppManager::renderNewScene() {
	auto data = getNetworkData();
	Renderer::getInstance().loadNN(data.first, data.second);
}

void AppManager::updateCurrentScene() {
	auto data = getNetworkData();
	Renderer::getInstance().updateNN(data.first, data.second);
}


