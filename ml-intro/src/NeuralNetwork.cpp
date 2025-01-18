#include "NeuralNetwork.h"
#include <iostream>
#include <cmath>
#include <numeric>

// Constructor
NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize,
    std::shared_ptr<Function> hiddenActivationFunction,
    std::shared_ptr<Function> outputActivationFunction)
    : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize),
    learningRate(0.05), bias(1.0),
    hiddenActivationFunction(hiddenActivationFunction), outputActivationFunction(outputActivationFunction)
     {

    // Initialize weights for the hidden layer
    if (hiddenSize > 0) {
        // Inicializácia váh pre skrytú a výstupnú vrstvu
        hiddenWeights = Eigen::MatrixXd::Random(hiddenSize, inputSize);
        outputWeights = Eigen::MatrixXd::Random(outputSize, hiddenSize);
    }
    else {
        // Ak hiddenSize = 0, výstupná vrstva priamo spracováva vstupy
        outputWeights = Eigen::MatrixXd::Random(outputSize, inputSize);
    }
}

// Predict output for given inputs
Eigen::VectorXd NeuralNetwork::predict(const Eigen::VectorXd& inputs) {
    Eigen::VectorXd finalOutput;

    if (hiddenSize > 0) {
        Eigen::VectorXd hiddenInput = hiddenWeights * inputs + Eigen::VectorXd::Constant(hiddenSize, bias);
        Eigen::VectorXd hiddenOutput = hiddenInput.unaryExpr([this](double x) {
            return hiddenActivationFunction->function(x);
            });

        Eigen::VectorXd outputInput = outputWeights * hiddenOutput + Eigen::VectorXd::Constant(outputSize, bias);
        finalOutput = outputInput.unaryExpr([this](double x) {
            return outputActivationFunction ? outputActivationFunction->function(x) : x;
            });
    }
    else {
        Eigen::VectorXd outputInput = outputWeights * inputs + Eigen::VectorXd::Constant(outputSize, bias);
        finalOutput = outputInput.unaryExpr([this](double x) {
            return outputActivationFunction ? outputActivationFunction->function(x) : x;
            });
    }

    if (!outputActivationFunction) {
        finalOutput = softmax(finalOutput);
    }

    return finalOutput;
}


void NeuralNetwork::train(const Eigen::VectorXd& inputs, const Eigen::VectorXd& targets) {
    Eigen::VectorXd finalOutput;
    Eigen::VectorXd hiddenOutput;

    if (hiddenSize > 0) {
        Eigen::VectorXd hiddenInput = hiddenWeights * inputs + Eigen::VectorXd::Constant(hiddenSize, bias);
        hiddenOutput = hiddenInput.unaryExpr([this](double x) {
            return hiddenActivationFunction->function(x);
            });
    }

    Eigen::VectorXd outputInput = (hiddenSize > 0 ? outputWeights * hiddenOutput : outputWeights * inputs) +
        Eigen::VectorXd::Constant(outputSize, bias);
    finalOutput = outputInput.unaryExpr([this](double x) {
        return outputActivationFunction ? outputActivationFunction->function(x) : x;
        });

    if (!outputActivationFunction) {
        finalOutput = softmax(finalOutput);
    }

    Eigen::VectorXd outputErrors = finalOutput - targets;
    if (outputActivationFunction) {
        outputErrors = outputErrors.array() * outputInput.unaryExpr([this](double x) {
            return outputActivationFunction->derivative(x);
            }).array();
    }

    if (hiddenSize > 0) {
        outputWeights -= learningRate * (outputErrors * hiddenOutput.transpose());
    }
    else {
        outputWeights -= learningRate * (outputErrors * inputs.transpose());
    }

    if (hiddenSize > 0) {
        Eigen::VectorXd hiddenErrors = outputWeights.transpose() * outputErrors;
        hiddenErrors = hiddenErrors.array() * hiddenOutput.unaryExpr([this](double x) {
            return hiddenActivationFunction->derivative(x);
            }).array();
            hiddenWeights -= learningRate * (hiddenErrors * inputs.transpose());
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
void NeuralNetwork::setHiddenActivationFunction(std::shared_ptr<Function> activationFunction) {
    hiddenActivationFunction = activationFunction;
}

// Set the activation function for the output layer
void NeuralNetwork::setOutputActivationFunction(std::shared_ptr<Function> activationFunction) {
    outputActivationFunction = activationFunction;
}

// Get the current bias value
double NeuralNetwork::getBias() const {
    return bias;
}

// Set a new bias value for the network
void NeuralNetwork::setBias(double newBias) {
    bias = newBias;
}

// Get the current learning rate
double NeuralNetwork::getLearningRate() const {
    return learningRate;
}

// Set a new learning rate for the network
void NeuralNetwork::setLearningRate(double newLearningRate) {
    learningRate = newLearningRate;
}

// Apply the softmax function to a vector
Eigen::VectorXd NeuralNetwork::softmax(const Eigen::VectorXd& logits) const{
    Eigen::VectorXd expLogits = (logits.array() - logits.maxCoeff()).exp(); // For numerical stability
    return expLogits / expLogits.sum(); // Normalize by the sum of exponents
}

void NeuralNetwork::saveNetwork(const std::string& filename) {
    nlohmann::json networkJson;

    // Save general network properties
    networkJson["inputSize"] = inputSize;
    networkJson["hiddenSize"] = hiddenSize;
    networkJson["outputSize"] = outputSize;
    networkJson["learningRate"] = learningRate;
    networkJson["bias"] = bias;

    // Save activation function names
    networkJson["hiddenActivationFunction"] = hiddenActivationFunction ? hiddenActivationFunction->name() : "None";
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
    learningRate = networkJson["learningRate"];
    bias = networkJson["bias"];

    // Load activation functions
    std::string hiddenActivationName = networkJson["hiddenActivationFunction"];
    std::string outputActivationName = networkJson["outputActivationFunction"];

    if (hiddenActivationName == "ReLu") {
        hiddenActivationFunction = std::make_shared<ReLu>();
    }
    else if (hiddenActivationName == "Sigmoid") {
        hiddenActivationFunction = std::make_shared<Sigmoid>();
    }
    else if (hiddenActivationName == "Tanh") {
        hiddenActivationFunction = std::make_shared<Tanh>();
    }
    else {
        hiddenActivationFunction = std::make_shared<ReLu>();
    }

    if (outputActivationName == "ReLu") {
        outputActivationFunction = std::make_shared<ReLu>();
    }
    else if (outputActivationName == "Sigmoid") {
        outputActivationFunction = std::make_shared<Sigmoid>();
    }
    else if (outputActivationName == "Tanh") {
        outputActivationFunction = std::make_shared<Tanh>();
    }
    else if (outputActivationName == "SoftMax") {
        outputActivationFunction = nullptr;
    }
    else {
        outputActivationFunction = std::make_shared<ReLu>();
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
    return outputActivationFunction->name();
}
std::pair<std::vector<std::vector<float>>, std::vector<std::vector<float>>> NeuralNetwork::extractNetworkData(const Eigen::VectorXd& input) const {
    std::vector<std::vector<float>> layers;  // Store activations
    std::vector<std::vector<float>> weights; // Store weights

    // Input layer activations
    std::vector<float> inputActivations(input.data(), input.data() + input.size());
    layers.emplace_back(inputActivations);

    Eigen::VectorXd hiddenOutput;
    if (hiddenSize > 0) {
        // Hidden layer: compute activations
        Eigen::VectorXd hiddenInput = hiddenWeights * input + Eigen::VectorXd::Constant(hiddenSize, bias);
        hiddenOutput = hiddenInput.unaryExpr([this](double x) {
            return hiddenActivationFunction->function(x);
            });

        std::vector<float> hiddenActivations(hiddenOutput.data(), hiddenOutput.data() + hiddenOutput.size());
        layers.emplace_back(hiddenActivations);
    }

    // Output layer: compute activations
    Eigen::VectorXd outputInput = (hiddenSize > 0 ? outputWeights * hiddenOutput : outputWeights * input) + Eigen::VectorXd::Constant(outputSize, bias);
    Eigen::VectorXd finalOutput = outputInput.unaryExpr([this](double x) {
        return outputActivationFunction ? outputActivationFunction->function(x) : x;
        });

    if (!outputActivationFunction) {
        finalOutput = softmax(finalOutput);
    }

    std::vector<float> outputActivations(finalOutput.data(), finalOutput.data() + finalOutput.size());
    layers.emplace_back(outputActivations);

    // Convert hidden weights
    if (hiddenSize > 0) {
        for (int i = 0; i < hiddenWeights.rows(); ++i) {
            std::vector<float> row(hiddenWeights.row(i).data(), hiddenWeights.row(i).data() + hiddenWeights.cols());
            weights.emplace_back(row);
        }
    }

    // Convert output weights
    for (int i = 0; i < outputWeights.rows(); ++i) {
        std::vector<float> row(outputWeights.row(i).data(), outputWeights.row(i).data() + outputWeights.cols());
        weights.emplace_back(row);
    }

    return { layers, weights };
}

