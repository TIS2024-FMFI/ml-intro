#include "AppManager.h"
#include <iomanip>

AppManager::AppManager()
{
	gui = new ImGuiApp(*this, GetModuleHandle(nullptr));

	networks[1] = std::make_unique<NeuralNetwork>( 2      , 0  , 1 , &gui->activationFunctionHidden, &gui->activationFunctionOutput, &gui->bias, &gui->learningRate);
	networks[2] = std::make_unique<NeuralNetwork>( 3      , 5  , 7 , &gui->activationFunctionHidden, &gui->activationFunctionOutput, &gui->bias, &gui->learningRate);
	networks[3] = std::make_unique<NeuralNetwork>( 28 * 28, 128, 10, &gui->activationFunctionHidden, &gui->activationFunctionOutput, &gui->bias, &gui->learningRate);
}

AppManager::~AppManager()
{
	delete gui;
}

void AppManager::run()
{
	if (!gui->Initialize()) {
		MessageBox(nullptr, _T("Failed to initialize ImGui application"), _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	gui->Run();
}



void AppManager::runNetwork()
{
	std::vector<Eigen::VectorXd> trainingData;
	std::vector<Eigen::VectorXd> targets;
	int currScenario = gui->getCurrentScenrio();
	int epochs = gui->getEpochs();

	switch (currScenario) {
	case 1: {
		auto nN1 = getCurrentNetwork();
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


		nN1->fit(trainingData, targets, epochs);
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
	}
	case 2: {
		auto nN2 = getCurrentNetwork();
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


		nN2->fit(trainingData, targets, epochs);
		//sendDataToRenderer(trainingData.back());
		std::cout << "Predictions after training:\n";
		for (size_t i = 0; i < trainingData.size(); ++i) {
			Eigen::VectorXd prediction = nN2->predict(trainingData[i]);
			std::cout << std::fixed << std::setprecision(3);
			std::cout << "Input: " << trainingData[i].transpose() << " Prediction: " << prediction.transpose()
				<< std::setprecision(0) << " Target: " << targets[i].transpose() << "\n";
		}
		break;
	}
	case 3: {
		auto nN3 = getCurrentNetwork();
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
		nN3->fit(trainingData, targets, epochs);
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



void AppManager::saveNetwork()
{
	int scen_id = gui->getCurrentScenrio();
	getCurrentNetwork()->saveNetwork("xd" + scen_id);
}

void AppManager::loadNetwork()
{
	getCurrentNetwork()->loadNetwork(openFileDialog());
}

// TODO
Eigen::VectorXd AppManager::setNetworkInput()
{
	auto guiInput = gui->getInput();
	Eigen::VectorXd eigenVec(guiInput.size());
	std::copy(guiInput.begin(), guiInput.end(), eigenVec.data());
	return eigenVec;
}

void AppManager::tellOutput(int output)
{
	Eigen::VectorXd inputs = setNetworkInput();

	int currScenario = gui->getCurrentScenrio();
	int result = -1;
	switch (currScenario) {
	case 1: {
		auto nN1 = getCurrentNetwork();
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
		std::cout << "RESULT: " << result << "\n";
		break;
	}

	case 2: {
		auto nN2 = getCurrentNetwork();
		Eigen::VectorXd outputVector = Eigen::VectorXd::Zero(7);
		if (output >= 0 && output < 7) {
			outputVector[output] = 1.0;
		}

		nN2->train(inputs, outputVector);
		Eigen::VectorXd prediction = nN2->predict(inputs);
		result = static_cast<int>(std::distance(prediction.data(), std::max_element(prediction.data(), prediction.data() + 7)));

		std::cout << "RESULT: " << result << "\n";
		break;
	}
	case 3: {
		auto nN3 = getCurrentNetwork();
		Eigen::VectorXd outputVector = Eigen::VectorXd::Zero(10);
		if (output >= 0 && output < 10) {
			outputVector[output] = 1.0;
		}

		nN3->train(inputs, outputVector);
		Eigen::VectorXd prediction = nN3->predict(inputs);
		result = static_cast<int>(std::distance(prediction.data(), std::max_element(prediction.data(), prediction.data() + 10)));

		std::cout << "RESULT: " << result << "\n";

		break;
	}

	default:
		break;
	}

	//int inputSize = getCurrentNetwork()->getInputSize();
	//Eigen::VectorXd outputVector = Eigen::VectorXd::Zero(inputSize);
	//if (output >= 0 && output < inputSize) {
	//	outputVector[output] = 1.0;
	//}

	//getCurrentNetwork()->train(inputs, outputVector);
	//Eigen::VectorXd prediction = getCurrentNetwork()->predict(inputs);
	//result = static_cast<int>(std::distance(prediction.data(), std::max_element(prediction.data(), prediction.data() + inputSize))); //prediction.end() by nefungovalo? (neviem co je vo vnutry)
  
	updateCurrentScene();
	gui->setOuput(result);
}


void AppManager::renderNewScene() {
	auto data = getCurrentNetwork()->extractNetworkData();
	Renderer::getInstance().loadNN(data.first, data.second);
}

void AppManager::predictCurrentGuiInput() {
	int inputSize = getCurrentNetwork()->getInputSize();
	Eigen::VectorXd prediction = getCurrentNetwork()->predict(setNetworkInput());
	auto inpt = setNetworkInput();
	//std::cout << prediction[0] << ", " << prediction[1] << std::endl;
	int result = static_cast<int>(std::distance(prediction.data(), std::max_element(prediction.data(), prediction.data() + inputSize)));
	gui->setOuput(result);
}

void AppManager::updateCurrentScene() {
	auto data = getCurrentNetwork()->extractNetworkData();
	Renderer::getInstance().updateNN(data.first, data.second);
}

NeuralNetwork* AppManager::getCurrentNetwork() {
	int currScenario = gui->getCurrentScenrio();
	if (networks.find(currScenario) != networks.end()) {
		return networks[currScenario].get();
	}
	throw std::exception("valid scenario values are [1, 2, 3]");
}