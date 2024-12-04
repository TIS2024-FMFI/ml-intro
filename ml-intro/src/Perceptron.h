#pragma once
#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <vector>
#include <iostream>
#include <functional> // For std::function

class Perceptron {
private:
    std::vector<double> weights;
    double bias;
    double learningRate;

    // Activation function and its derivative
    std::function<double(double)> activationFn;
    std::function<double(double)> activationFnDerivative;

public:
    // Constructor
    Perceptron(int numInputs, double learningRate = 0.01,
        std::function<double(double)> activation = nullptr,
        std::function<double(double)> activationDerivative = nullptr);

    // Predict output
    double guess(const std::vector<double>& inputs) const;

    // Train the perceptron
    void train(const std::vector<double>& trainingData, double target);

    // Fit on a dataset
    void fit(const std::vector<std::vector<double>>& trainingData, const std::vector<double>& targets, int num_epochs);

    // Print weights and bias
    void printModel() const;

    // Set activation function and its derivative
    void setActivationFunction(std::function<double(double)> activation,
        std::function<double(double)> activationDerivative);
};



#endif //PERCEPTRON_H