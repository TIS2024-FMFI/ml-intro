#include "Perceptron.h"



// Constructor
Perceptron::Perceptron(int numInputs,
    std::shared_ptr<Function> activationFunc)
    : learningRate(0.05), activationFunction(activationFunc), bias(1.0) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-1.0, 1.0);

    for (int i = 0; i < numInputs+1; ++i) {
        weights.push_back(dist(gen));
    }

}

// Predict output
double Perceptron::guess(const std::vector<double>& inputs) const {
    if (inputs.size() + 1 != weights.size()) {
        throw std::invalid_argument("Input size does not match the number of weights.");
    }
    double weightedSum = 0.0;
    for (size_t i = 0; i < inputs.size(); ++i) {
        weightedSum += inputs[i] * weights[i];
    }
    weightedSum += bias * weights.back();
    if (activationFunction) {
        return activationFunction->function(weightedSum);
    }
    return weightedSum;
}

// Train the perceptron
void Perceptron::train(const std::vector<double>& trainingData, double target) {
    double prediction = guess(trainingData);
    double error = target - prediction;
    double delta = error;

    if (activationFunction) {
        delta = error * activationFunction->derivative(prediction);
    }
    for (size_t i = 0; i < trainingData.size(); ++i) {
        weights[i] += learningRate * delta * trainingData[i];
    }
    weights.back() += learningRate * delta * bias;
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
void Perceptron::setActivationFunction(std::shared_ptr<Function> newActivationFunction) {
    activationFunction = newActivationFunction;
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

std::vector<double> Perceptron::getWeights() const {
    return weights;
}

void Perceptron::setWeights(const std::vector<double>& newWeights) {
    if (newWeights.size() != weights.size()) {
        throw std::invalid_argument("Mismatch in weights size.");
    }
    weights = newWeights;
}