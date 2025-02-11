#include "NeuralNetwork.h"
#include <iostream>
#include <cmath>
#include <numeric>

// Constructor
NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize,
    int* hiddenActivationFunction, int* outputActivationFunction,
    double* bias_ptr, double* learningRate_ptr)
    : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize),
    learningRate(learningRate_ptr), bias(bias_ptr), lambda(0.01),
    hiddenActivationFunction(hiddenActivationFunction), outputActivationFunction(outputActivationFunction)
     {

    // Initialize weights for the hidden layer
    if (hiddenSize > 0) {
        // Inicializácia váh pre skrytú a výstupnú vrstvu
        hiddenWeights = Eigen::MatrixXd::Random(hiddenSize, inputSize) * sqrt(2.0 / inputSize);
        outputWeights = Eigen::MatrixXd::Random(outputSize, hiddenSize)* sqrt(2.0 / hiddenSize);
    }
    else {
        // Ak hiddenSize = 0, výstupná vrstva priamo spracováva vstupy
        outputWeights = Eigen::MatrixXd::Random(outputSize, inputSize) * sqrt(2.0 / inputSize);
    }


    lastInput = Eigen::VectorXd::Zero(inputSize);
}

// Predict output for given inputs
Eigen::VectorXd NeuralNetwork::predict(const Eigen::VectorXd& inputs) {
    Eigen::VectorXd finalOutput;
    lastInput = inputs;

    if (hiddenSize > 0) {
        Eigen::VectorXd hiddenInput = hiddenWeights * inputs + Eigen::VectorXd::Constant(hiddenSize, *bias);
        Eigen::VectorXd hiddenOutput = hiddenInput.unaryExpr([this](double x) {
            return listFunctions[*hiddenActivationFunction]->function(x);
            });

        Eigen::VectorXd outputInput = outputWeights * hiddenOutput + Eigen::VectorXd::Constant(outputSize, *bias);
        finalOutput = outputInput.unaryExpr([this](double x) {
            return listFunctions[*outputActivationFunction]->function(x);
            });
    }
    else {
        Eigen::VectorXd outputInput = outputWeights * inputs + Eigen::VectorXd::Constant(outputSize, *bias);
        finalOutput = outputInput.unaryExpr([this](double x) {
            return listFunctions[*outputActivationFunction]->function(x);
            });
    }

    if (*outputActivationFunction == 3) {
        finalOutput = softmax(finalOutput);
    }

    return finalOutput;
}


void NeuralNetwork::train(const Eigen::VectorXd& inputs, const Eigen::VectorXd& targets) {
    Eigen::VectorXd finalOutput;
    Eigen::VectorXd hiddenOutput;
    lastInput = inputs;

    if (hiddenSize > 0) {
        Eigen::VectorXd hiddenInput = hiddenWeights * inputs + Eigen::VectorXd::Constant(hiddenSize, *bias);
        hiddenOutput = hiddenInput.unaryExpr([this](double x) {
            return listFunctions[*hiddenActivationFunction]->function(x);
            });
    }

    Eigen::VectorXd outputInput = (hiddenSize > 0 ? outputWeights * hiddenOutput : outputWeights * inputs) +
        Eigen::VectorXd::Constant(outputSize, *bias);
    finalOutput = outputInput.unaryExpr([this](double x) {
        return listFunctions[*outputActivationFunction]->function(x);
        });

    if (*outputActivationFunction == 3) {
        finalOutput = softmax(finalOutput);
    }

    Eigen::VectorXd outputErrors = finalOutput - targets;
    if (*outputActivationFunction != 3) {
        outputErrors = outputErrors.array() * outputInput.unaryExpr([this](double x) {
            return listFunctions[*outputActivationFunction]->derivative(x);
            }).array();
    }

    // Update output weights with L2 regularization
    if (hiddenSize > 0) {
        Eigen::MatrixXd gradientOutput = outputErrors * hiddenOutput.transpose();
        outputWeights -= *learningRate * (gradientOutput + lambda * outputWeights); // Add L2 penalty
    }
    else {
        Eigen::MatrixXd gradientOutput = outputErrors * inputs.transpose();
        outputWeights -= *learningRate * (gradientOutput + lambda * outputWeights); // Add L2 penalty
    }

    // Update hidden weights with L2 regularization
    if (hiddenSize > 0) {
        Eigen::VectorXd hiddenErrors = outputWeights.transpose() * outputErrors;
        hiddenErrors = hiddenErrors.array() * hiddenOutput.unaryExpr([this](double x) {
            return listFunctions[*hiddenActivationFunction]->derivative(x);
            }).array();

            Eigen::MatrixXd gradientHidden = hiddenErrors * inputs.transpose();
            hiddenWeights -= *learningRate * (gradientHidden + lambda * hiddenWeights); // Add L2 penalty
    }
}


// Fit the neural network on a dataset
void NeuralNetwork::fit(const std::vector<Eigen::VectorXd>& trainingData,
    const std::vector<Eigen::VectorXd>& targets,
    int num_epochs) {

    for (int epoch = 0; epoch < num_epochs; ++epoch) {
        int correctPredictions = 0;
        for (size_t i = 0; i < trainingData.size(); ++i) {
            train(trainingData[i], targets[i]);

            // Check prediction accuracy
            Eigen::VectorXd prediction = predict(trainingData[i]);

            size_t predictedClass = std::distance(prediction.data(), std::max_element(prediction.data(), prediction.data() + prediction.size()));
            size_t targetClass = std::distance(targets[i].data(), std::max_element(targets[i].data(), targets[i].data() + targets[i].size()));

            if (predictedClass == targetClass) {
                correctPredictions++;
            }
        }

        // Print accuracy for each epoch
        double accuracy = (static_cast<double>(correctPredictions) / trainingData.size()) * 100.0;
        std::cout << "Epoch " << epoch + 1 << ": Accuracy: " << accuracy << "%" << std::endl;
    }
}

// Set the activation function for the hidden layer
void NeuralNetwork::setHiddenActivationFunction(int activationFunction) {
    *hiddenActivationFunction = activationFunction;
}

// Set the activation function for the output layer
void NeuralNetwork::setOutputActivationFunction(int activationFunction) {
    *outputActivationFunction = activationFunction;
}

// Get the current bias value
double NeuralNetwork::getBias() const {
    return *bias;
}

// Set a new bias value for the network
void NeuralNetwork::setBias(double newBias) {
    *bias = newBias;
}

// Get the current learning rate
double NeuralNetwork::getLearningRate() const {
    return *learningRate;
}

// Set a new learning rate for the network
void NeuralNetwork::setLearningRate(double newLearningRate) {
    *learningRate = newLearningRate;
}

// Apply the softmax function to a vector
Eigen::VectorXd NeuralNetwork::softmax(const Eigen::VectorXd& logits) const{
    /*std::cout << "Logits: ";
    for (int i = 0; i < logits.size(); ++i) {
        std::cout << logits[i] << " ";
    }
    std::cout << std::endl;*/
    Eigen::VectorXd expLogits = (logits.array() - logits.maxCoeff()).exp(); // For numerical stability
    return expLogits / expLogits.sum(); // Normalize by the sum of exponents
}

void NeuralNetwork::saveNetwork(const std::string& filename) {
    nlohmann::json networkJson;

    // Save general network properties
    networkJson["inputSize"] = inputSize;
    networkJson["hiddenSize"] = hiddenSize;
    networkJson["outputSize"] = outputSize;
    networkJson["learningRate"] = *learningRate;
    networkJson["bias"] = *bias;

    // Save activation function names
    networkJson["hiddenActivationFunction"] = listFunctions[*hiddenActivationFunction]->name();
    networkJson["outputActivationFunction"] = getActivationFuncName();

    // Save hidden layer weights
    if (hiddenSize > 0) {
        networkJson["hiddenWeights"] = std::vector<std::vector<double>>(hiddenWeights.rows());
        for (int i = 0; i < hiddenWeights.rows(); ++i) {
            networkJson["hiddenWeights"][i] = std::vector<double>(hiddenWeights.row(i).data(), hiddenWeights.row(i).data() + hiddenWeights.cols());
        }
    }

    // Save output layer weights
    networkJson["outputWeights"] = std::vector<std::vector<double>>(outputWeights.rows());
    for (int i = 0; i < outputWeights.rows(); ++i) {
        networkJson["outputWeights"][i] = std::vector<double>(outputWeights.row(i).data(), outputWeights.row(i).data() + outputWeights.cols());
    }

    // Save to a file
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for saving: " + filename);
    }
    file << networkJson.dump(4); // Pretty print with 4 spaces
    file.close();
    std::cout << "Network saved to " << filename << "\n";
}

void NeuralNetwork::loadNetwork(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for loading: " + filename);
    }

    nlohmann::json networkJson;
    file >> networkJson;
    file.close();

    // Validate the JSON structure
    if (!networkJson.contains("inputSize") ||
        !networkJson.contains("hiddenSize") ||
        !networkJson.contains("outputSize") ||
        !networkJson.contains("learningRate") ||
        !networkJson.contains("bias") ||
        !networkJson.contains("outputWeights") ||
        !networkJson.contains("hiddenActivationFunction") ||
        !networkJson.contains("outputActivationFunction")) {
        throw std::runtime_error("Invalid network JSON structure.");
    }

    // Validate dimensions
    int fileInputSize = networkJson["inputSize"];
    int fileHiddenSize = networkJson["hiddenSize"];
    int fileOutputSize = networkJson["outputSize"];

    if (fileInputSize != inputSize || fileHiddenSize != hiddenSize || fileOutputSize != outputSize) {
        std::cerr << "Error: Network dimensions do not match.\n"
            << "Expected (Input: " << inputSize << ", Hidden: " << hiddenSize << ", Output: " << outputSize << ")\n"
            << "Got (Input: " << fileInputSize << ", Hidden: " << fileHiddenSize << ", Output: " << fileOutputSize << ")\n";
        return; // Exit without loading
    }

    // Load general properties
    *learningRate = networkJson["learningRate"];
    *bias = networkJson["bias"];

    // Load activation functions
    std::string hiddenActivationName = networkJson["hiddenActivationFunction"];
    std::string outputActivationName = networkJson["outputActivationFunction"];

    if (hiddenActivationName == "ReLu") {
        *hiddenActivationFunction = 0;
    }
    else if (hiddenActivationName == "Sigmoid") {
        *hiddenActivationFunction = 1;
    }
    else if (hiddenActivationName == "Tanh") {
        *hiddenActivationFunction = 2;
    }
    else {
        *hiddenActivationFunction = 0;
    }

    if (outputActivationName == "ReLu") {
        *outputActivationFunction = 0;
    }
    else if (outputActivationName == "Sigmoid") {
        *outputActivationFunction = 1;
    }
    else if (outputActivationName == "Tanh") {
        *outputActivationFunction = 2;
    }
    else {
        *outputActivationFunction = 3;
    }

    // Load hidden layer weights
    if (hiddenSize > 0 && networkJson.contains("hiddenWeights")) {
        hiddenWeights = Eigen::MatrixXd(hiddenSize, inputSize);
        for (int i = 0; i < hiddenSize; ++i) {
            std::vector<double> row = networkJson["hiddenWeights"][i];
            for (int j = 0; j < inputSize; ++j) {
                hiddenWeights(i, j) = row[j];
            }
        }
    }

    // Load output layer weights
    outputWeights = Eigen::MatrixXd(outputSize, hiddenSize > 0 ? hiddenSize : inputSize);
    for (int i = 0; i < outputSize; ++i) {
        std::vector<double> row = networkJson["outputWeights"][i];
        for (int j = 0; j < (hiddenSize > 0 ? hiddenSize : inputSize); ++j) {
            outputWeights(i, j) = row[j];
        }
    }

    std::cout << "Network successfully loaded from " << filename << "\n";
}


std::string NeuralNetwork::getActivationFuncName() {
    if (outputActivationFunction == nullptr) {
        return "SoftMax";
    }
    return listFunctions[*outputActivationFunction]->name();
}
std::pair<std::vector<Eigen::MatrixXd>, std::vector<Eigen::MatrixXd>> NeuralNetwork::extractNetworkData() const {

    Eigen::VectorXd input = lastInput;
    std::vector<Eigen::MatrixXd> layers;  // Store activations as Eigen matrices
    std::vector<Eigen::MatrixXd> weights; // Store weights as Eigen matrices

    // Input layer activations
    Eigen::MatrixXd inputActivations = input; // Store input as a single-column matrix
    layers.push_back(inputActivations);


    Eigen::VectorXd hiddenOutput;
    if (hiddenSize > 0) {
        // Hidden layer: compute activations
        Eigen::VectorXd hiddenInput = hiddenWeights * input + Eigen::VectorXd::Constant(hiddenSize, *bias);
        hiddenOutput = hiddenInput.unaryExpr([this](double x) {
            return listFunctions[*hiddenActivationFunction]->function(x);
            });

        Eigen::MatrixXd hiddenActivations = hiddenOutput; // Store hidden layer activations
        layers.push_back(hiddenActivations);
    }

    // Output layer: compute activations
    Eigen::VectorXd outputInput = (hiddenSize > 0 ? outputWeights * hiddenOutput : outputWeights * input) + Eigen::VectorXd::Constant(outputSize, *bias);
    Eigen::VectorXd finalOutput = outputInput.unaryExpr([this](double x) {
        return listFunctions[*outputActivationFunction]->function(x);
        });

    if (*outputActivationFunction == 3) {
        finalOutput = softmax(finalOutput);
    }

    Eigen::MatrixXd outputActivations = finalOutput; // Store output layer activations
    layers.push_back(outputActivations);

    // Store hidden weights
    if (hiddenSize > 0) {
        weights.push_back(hiddenWeights); // Add hidden weights as Eigen matrix
    }

    // Store output weights
    weights.push_back(outputWeights); // Add output weights as Eigen matrix



    return { layers, weights };
}


