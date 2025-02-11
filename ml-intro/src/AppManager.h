#pragma once

#include "NeuralNetwork.h"
#include "ImGuiApp.h"
#include "FileDialog.h"
#include "trainingSet.h"

class AppManager
{
private:
	ImGuiApp* gui;
	std::map<int, std::shared_ptr<NeuralNetwork>> networks;

	NeuralNetwork* getCurrentNetwork();

public:
	AppManager();
	~AppManager();

	void run();

	void runNetwork();
	void saveNetwork();
	void loadNetwork();
	Eigen::VectorXd setNetworkInput();

	void tellOutput(int output); 
	void predictCurrentGuiInput();

	void renderNewScene();
	void updateCurrentScene();

	void resetNetworkToRandomValues();
};

