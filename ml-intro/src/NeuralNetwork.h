#pragma once
#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#define EIGEN_USE_THREADS

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
    double lambda;

    double* bias;
    double* learningRate;
    int* outputActivationFunction;
    int* hiddenActivationFunction;

    Eigen::VectorXd lastInput;

    // Weights and biases represented as matrices/vectors
    Eigen::MatrixXd hiddenWeights;
    Eigen::MatrixXd outputWeights;


    // Helper to compute layer outputs
    Eigen::VectorXd computeLayerOutput(const Eigen::MatrixXd& weights, const Eigen::VectorXd& biases,
        const Eigen::VectorXd& inputs, const std::shared_ptr<Function>& activationFunction);

    std::vector<std::shared_ptr<Function>> listFunctions = { std::make_shared<ReLu>() , std::make_shared<Sigmoid>(), std::make_shared<Tanh>(), std::make_shared<SoftMax>() };

public:
    // Constructor
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize,
        int* hiddenActivationFunction, int* outputActivationFunction,
        double* bias_Ptr, double* learningRate_Ptr);

    // Predict output for given inputs
    Eigen::VectorXd predict(const Eigen::VectorXd& inputs);

    // Train the neural network
    void train(const Eigen::VectorXd& inputs, const Eigen::VectorXd& targets);

    // Fit on a dataset
    void fit(const std::vector<Eigen::VectorXd>& trainingData,
        const std::vector<Eigen::VectorXd>& targets,
        int num_epochs);

    // Setters for activation functions
    void setHiddenActivationFunction(int activationFunction);
    void setOutputActivationFunction(int activationFunction);
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
    int getHiddenSize() { return hiddenSize; }
    int getOutputSize() { return outputSize; }
};

#endif // NEURALNETWORK_H
