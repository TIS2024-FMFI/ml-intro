#pragma once

#include "NeuralNetwork.h"
#include "ImGuiApp.h"
#include "FileDialog.h"
#include "trainingSet.h"

class AppManager
{
private:
	ImGuiApp* gui;
	NeuralNetwork* nN1;
	NeuralNetwork* nN2;
	NeuralNetwork* nN3;
	NeuralNetwork* testing;

public:
	AppManager();
	~AppManager();

	void run();

	void runNetwork();
	void saveNetwork();
	void loadNetwork();
	void setNetworkInput();
	void setNetworkBias(float bias);
	void setNetworkLearningRate(float learningRate);
	void setNetworkActivationFunction(std::shared_ptr<Function> activationFunctionHidden, std::shared_ptr<Function> activationFunctionOutput);
	int tellOutput(int output);

};

