#pragma once
#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "functions.cpp"
#include <Eigen/Dense>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <chrono>

class NeuralNetwork {
private:
    int inputSize;
    int hiddenSize;
    int outputSize;
    double learningRate;
    double bias;
    double lambda;

    Eigen::VectorXd lastInput;

    // Weights and biases represented as matrices/vectors
    Eigen::MatrixXd hiddenWeights;
    Eigen::MatrixXd outputWeights;

    std::shared_ptr<Function> outputActivationFunction;
    std::shared_ptr<Function> hiddenActivationFunction;

    // Helper to compute layer outputs
    Eigen::VectorXd computeLayerOutput(const Eigen::MatrixXd& weights, const Eigen::VectorXd& biases,
        const Eigen::VectorXd& inputs, const std::shared_ptr<Function>& activationFunction);

public:
    // Constructor
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize,
        std::shared_ptr<Function> hiddenActivationFunction ,
        std::shared_ptr<Function> outputActivationFunction);

    // Predict output for given inputs
    Eigen::VectorXd predict(const Eigen::VectorXd& inputs);

    // Train the neural network
    void train(const Eigen::VectorXd& inputs, const Eigen::VectorXd& targets);

    // Fit on a dataset
    void fit(const std::vector<Eigen::VectorXd>& trainingData,
        const std::vector<Eigen::VectorXd>& targets,
        int num_epochs);

    // Setters for activation functions
    void setHiddenActivationFunction(std::shared_ptr<Function> activationFunction);
    void setOutputActivationFunction(std::shared_ptr<Function> activationFunction);
    std::string getActivationFuncName();
    // Getters and setters for bias
    double getBias() const;
    void setBias(double newBias);

    // Getters and setters for learning rate
    double getLearningRate() const;
    void setLearningRate(double newLearningRate);

    // Save and load network state
    void saveNetwork(const std::string& filename);
    void loadNetwork(const std::string& filename);

    // Utility function for softmax
    Eigen::VectorXd softmax(const Eigen::VectorXd& logits) const;

    std::pair<std::vector<Eigen::MatrixXd>, std::vector<Eigen::MatrixXd>> extractNetworkData() const;

    int getInputSize() { return inputSize; }
};

#endif // NEURALNETWORK_H
