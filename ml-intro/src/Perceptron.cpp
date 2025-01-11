#include "Perceptron.h"

#include <iostream>
#include <vector>
#include <random>
#include <cmath>

// Constructor
Perceptron::Perceptron(int numInputs, double learningRate,
    std::function<double(double)> activation,
    std::function<double(double)> activationDerivative)
    : learningRate(learningRate), activationFn(activation), activationFnDerivative(activationDerivative) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-1.0, 1.0);

    for (int i = 0; i < numInputs; ++i) {
        weights.push_back(dist(gen));
    }
    bias = dist(gen);

    // Default to step activation if none provided
    if (!activationFn) {
        activationFn = [](double x) { return x >= 0 ? 1 : -1; };
    }
    if (!activationFnDerivative) {
        activationFnDerivative = [](double) { return 1; }; // Placeholder for step activation
    }
}

// Predict output
double Perceptron::guess(const std::vector<double>& inputs) const {
    if (inputs.size() != weights.size()) {
        throw std::invalid_argument("Input size does not match the number of weights.");
    }

    double weightedSum = bias;
    for (size_t i = 0; i < inputs.size(); ++i) {
        weightedSum += inputs[i] * weights[i];
    }
    return activationFn(weightedSum);
}

// Train the perceptron
void Perceptron::train(const std::vector<double>& trainingData, double target) {
    double prediction = guess(trainingData);
    double error = target - prediction;

    // Update weights using the derivative of the activation function
    double delta = error * activationFnDerivative(prediction);
    for (size_t i = 0; i < weights.size(); ++i) {
        weights[i] += learningRate * delta * trainingData[i];
    }
    bias += learningRate * delta;
}

// Fit on a dataset
void Perceptron::fit(const std::vector<std::vector<double>>& trainingData, const std::vector<double>& targets, int num_epochs) {
    for (int epoch = 0; epoch < num_epochs; ++epoch) {
        for (size_t i = 0; i < trainingData.size(); ++i) {
            train(trainingData[i], targets[i]);
        }
    }
}

// Print weights and bias
void Perceptron::printModel() const {
    std::cout << "Weights: ";
    for (const auto& w : weights) {
        std::cout << w << " ";
    }
    std::cout << "\nBias: " << bias << std::endl;
}

// Set activation function and its derivative
void Perceptron::setActivationFunction(std::function<double(double)> activation,
    std::function<double(double)> activationDerivative) {
    activationFn = activation;
    activationFnDerivative = activationDerivative;
}

// Getter for bias
double Perceptron::getBias() const {
    return bias;
}

// Setter for bias
void Perceptron::setBias(double newBias) {
    bias = newBias;
}

// Getter for learning rate
double Perceptron::getLearningRate() const {
    return learningRate;
}

// Setter for learning rate
void Perceptron::setLearningRate(double newLearningRate) {
    learningRate = newLearningRate;
}