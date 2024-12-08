#include "NeuralNetwork.h"
#include <iostream>

// Constructor
NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize, double learningRate,
    std::function<double(double)> activation,
    std::function<double(double)> activationDerivative)
    : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize), learningRate(learningRate) {
    // Initialize hidden layer perceptrons
    for (int i = 0; i < hiddenSize; ++i) {
        hiddenLayer.emplace_back(inputSize, learningRate, activation, activationDerivative);
    }

    // Initialize output layer perceptrons
    for (int i = 0; i < outputSize; ++i) {
        outputLayer.emplace_back(hiddenSize, learningRate, activation, activationDerivative);
    }
}

// Helper to compute layer outputs
std::vector<double> NeuralNetwork::computeLayerOutput(const std::vector<Perceptron>& layer, const std::vector<double>& inputs) {
    std::vector<double> outputs;
    for (const auto& perceptron : layer) {
        outputs.push_back(perceptron.guess(inputs));
    }
    return outputs;
}

// Predict output for given inputs
std::vector<double> NeuralNetwork::predict(const std::vector<double>& inputs) {
    if (inputs.size() != inputSize) {
        throw std::invalid_argument("Input size does not match the network's input layer size.");
    }

    // Pass through the hidden layer
    std::vector<double> hiddenOutputs = computeLayerOutput(hiddenLayer, inputs);

    // Pass through the output layer
    return computeLayerOutput(outputLayer, hiddenOutputs);
}

// Train the neural network
void NeuralNetwork::train(const std::vector<double>& inputs, const std::vector<double>& targets) {
    if (inputs.size() != inputSize || targets.size() != outputSize) {
        throw std::invalid_argument("Input or target size does not match the network's configuration.");
    }

    // Forward pass
    std::vector<double> hiddenOutputs = computeLayerOutput(hiddenLayer, inputs);
    std::vector<double> finalOutputs = computeLayerOutput(outputLayer, hiddenOutputs);

    // Calculate errors for output layer
    std::vector<double> outputErrors(outputLayer.size());
    for (size_t i = 0; i < outputLayer.size(); ++i) {
        outputErrors[i] = targets[i] - finalOutputs[i];
    }

    // Update output layer weights
    for (size_t i = 0; i < outputLayer.size(); ++i) {
        outputLayer[i].train(hiddenOutputs, targets[i]);
    }

    // Calculate errors for hidden layer
    std::vector<double> hiddenErrors(hiddenLayer.size(), 0.0);
    for (size_t i = 0; i < hiddenLayer.size(); ++i) {
        for (size_t j = 0; j < outputLayer.size(); ++j) {
            hiddenErrors[i] += outputErrors[j] * outputLayer[j].guess(hiddenOutputs); // Backpropagate error
        }
    }

    // Update hidden layer weights
    for (size_t i = 0; i < hiddenLayer.size(); ++i) {
        hiddenLayer[i].train(inputs, hiddenErrors[i]);
    }
}

// Fit on a dataset
void NeuralNetwork::fit(const std::vector<std::vector<double>>& trainingData,
    const std::vector<std::vector<double>>& targets,
    int num_epochs) {
    for (int epoch = 0; epoch < num_epochs; ++epoch) {
        for (size_t i = 0; i < trainingData.size(); ++i) {
            train(trainingData[i], targets[i]);
        }
    }
}


// Set activation function for all perceptrons in the network
void NeuralNetwork::setActivationFunction(std::function<double(double)> activation,
    std::function<double(double)> activationDerivative) {
    for (auto& perceptron : hiddenLayer) {
        perceptron.setActivationFunction(activation, activationDerivative);
    }
    for (auto& perceptron : outputLayer) {
        perceptron.setActivationFunction(activation, activationDerivative);
    }
}

// Print model information
void NeuralNetwork::printModel() const {
    std::cout << "Hidden Layer:" << std::endl;
    for (size_t i = 0; i < hiddenLayer.size(); ++i) {
        std::cout << "Perceptron " << i + 1 << ": ";
        hiddenLayer[i].printModel();
    }

    std::cout << "Output Layer:" << std::endl;
    for (size_t i = 0; i < outputLayer.size(); ++i) {
        std::cout << "Perceptron " << i + 1 << ": ";
        outputLayer[i].printModel();
    }
}