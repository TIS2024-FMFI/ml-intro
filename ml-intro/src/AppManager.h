#pragma once

#include "NeuralNetwork.h"
#include "ImGuiApp.h"
#include "FileDialog.h"
#include "trainingSet.h"

class AppManager
{
private:
	ImGuiApp* gui;
	//std::map<int, std::unique_ptr<NeuralNetwork>> networks;
	std::map<int, std::shared_ptr<NeuralNetwork>> networks;
	//NeuralNetwork* nN1;
	//NeuralNetwork* nN2;
	//NeuralNetwork* nN3;
	NeuralNetwork* testing;

	NeuralNetwork* getCurrentNetwork();

public:
	AppManager();
	~AppManager();

	void run();

	void runNetwork();
	void saveNetwork();
	void loadNetwork();
	Eigen::VectorXd setNetworkInput();
	void setNetworkBias(float bias);
	void setNetworkLearningRate(float learningRate);
	void setNetworkActivationFunction(std::shared_ptr<Function> activationFunctionHidden, std::shared_ptr<Function> activationFunctionOutput);

	void tellOutput(int output);

	void renderNewScene();
	void updateCurrentScene();


};

