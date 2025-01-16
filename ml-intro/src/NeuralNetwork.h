#pragma once
#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Perceptron.h"
#include <vector>
#include <functional>
#include "json.hpp"
#include <fstream>
#include "functions.cpp"


class NeuralNetwork {
private:
    int inputSize;
    int hiddenSize;
    int outputSize;
    double learningRate;
    double bias;

    // Layers of perceptrons
    std::vector<Perceptron> hiddenLayer;
    std::vector<Perceptron> outputLayer;

    std::function<double(double)> hiddenActivationFn;
    std::function<double(double)> hiddenActivationFnDerivative;
    std::function<double(double)> outputActivationFn;
    std::function<double(double)> outputActivationFnDerivative;



    // Helper to compute layer outputs
    std::vector<double> computeLayerOutput(const std::vector<Perceptron>& layer, const std::vector<double>& inputs, bool isOutputLayer);

public:
    // Constructor
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize,
        std::function<double(double)> hiddenActivationFn = nullptr,
    std::function<double(double)> hiddenActivationFnDerivative = nullptr,
    std::function<double(double)> outputActivationFn = nullptr,
    std::function<double(double)> outputActivationFnDerivative = nullptr);

    // Predict output for given inputs
    std::vector<double> predict(const std::vector<double>& inputs);

    // Train the neural network
    void train(const std::vector<double>& inputs, const std::vector<double>& targets);

    // Fit on a dataset
    void fit(const std::vector<std::vector<double>>& trainingData,
        const std::vector<std::vector<double>>& targets,
        int num_epochs);

    void setHiddenActivationFunction(std::function<double(double)> activation,
        std::function<double(double)> activationDerivative);
    void setOutputActivationFunction(std::function<double(double)> activation,
        std::function<double(double)> activationDerivative);

    // Print model information
    void printModel() const;

    // Get and set bias for all perceptrons
    double getBias() const;
    void setBias(double newBias);

    // Get and set learning rate for all perceptrons
    double getLearningRate() const;
    void setLearningRate(double newLearningRate);

    void saveNetwork(const std::string& filename);
    void loadNetwork(const std::string& filename);

    std::vector<double> softmax(const std::vector<double>& logits);


};

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    os << "[ ";
    for (const T& element : vec) {
        os << element << " ";
    }
    os << "]";
    return os;
}

#endif //NEURALNETWORK_H