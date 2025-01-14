#pragma once

#include "NeuralNetwork.h"
#include "ImGuiApp.h"

class AppManager
{
private:
	ImGuiApp* gui;
	NeuralNetwork* neuralNetwork;

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
	void setNetworkActivationFunction(Function activationFunction);

};

