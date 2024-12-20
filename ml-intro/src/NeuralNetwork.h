#pragma once
#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Perceptron.h"
#include <vector>
#include <functional>

class NeuralNetwork {
private:
    int inputSize;
    int hiddenSize;
    int outputSize;
    double learningRate;

    // Layers of perceptrons
    std::vector<Perceptron> hiddenLayer;
    std::vector<Perceptron> outputLayer;

    // Helper to compute layer outputs
    std::vector<double> computeLayerOutput(const std::vector<Perceptron>& layer, const std::vector<double>& inputs);

public:
    // Constructor
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize, double learningRate = 0.01,
        std::function<double(double)> activation = nullptr,
        std::function<double(double)> activationDerivative = nullptr);

    // Predict output for given inputs
    std::vector<double> predict(const std::vector<double>& inputs);

    // Train the neural network
    void train(const std::vector<double>& inputs, const std::vector<double>& targets);

    // Fit on a dataset
    void fit(const std::vector<std::vector<double>>& trainingData,
        const std::vector<std::vector<double>>& targets,
        int num_epochs);

    void setActivationFunction(std::function<double(double)> activation,
        std::function<double(double)> activationDerivative);

    // Print model information
    void printModel() const;
};


#endif //NEURALNETWORK_H