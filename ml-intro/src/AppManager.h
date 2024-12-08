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

};

