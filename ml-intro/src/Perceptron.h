#pragma once
#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <vector>
#include <iostream>
#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <functional> // For std::function
#include "functions.cpp"

class Perceptron {
private:
    std::vector<double> weights;
    double bias;
    double learningRate;
    std::shared_ptr<Function> activationFunction;



public:
    // Constructor
    Perceptron(int numInputs,
        std::shared_ptr<Function> activationFunc);

    // Predict output
    double guess(const std::vector<double>& inputs) const;

    // Train the perceptron
    void train(const std::vector<double>& trainingData, double target);

    // Fit on a dataset
    void fit(const std::vector<std::vector<double>>& trainingData, const std::vector<double>& targets, int num_epochs);

    // Print weights and bias
    void printModel() const;

    // Set activation function and its derivative
    void setActivationFunction(std::shared_ptr<Function> newActivationFunction);

    // Getter and setter for bias
    double getBias() const;
    void setBias(double newBias);

    // Getter and setter for learning rate
    double getLearningRate() const;
    void setLearningRate(double newLearningRate);
    std::vector<double> getWeights() const;
    void setWeights(const std::vector<double>& newWeights);
};



#endif //PERCEPTRON_H