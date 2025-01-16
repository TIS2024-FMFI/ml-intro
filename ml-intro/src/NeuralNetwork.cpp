#include "NeuralNetwork.h"
#include <iostream>

// Constructor
NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize,
    std::function<double(double)> hiddenActivationFn,
    std::function<double(double)> hiddenActivationFnDerivative,
    std::function<double(double)> outputActivationFn,
    std::function<double(double)> outputActivationFnDerivative)
    : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize), learningRate(0.05), bias(1.0), hiddenActivationFn(hiddenActivationFn), hiddenActivationFnDerivative(hiddenActivationFnDerivative),
    outputActivationFn(outputActivationFn),
    outputActivationFnDerivative(outputActivationFnDerivative){
    // Initialize hidden layer perceptrons
    for (int i = 0; i < hiddenSize; ++i) {
        hiddenLayer.emplace_back(inputSize, hiddenActivationFn, hiddenActivationFnDerivative);
    }

    // Initialize output layer perceptrons
    for (int i = 0; i < outputSize; ++i) {
        if (hiddenSize <= 0) {
            outputLayer.emplace_back(inputSize, outputActivationFn, outputActivationFnDerivative);
        }
        else {
            outputLayer.emplace_back(hiddenSize, outputActivationFn, outputActivationFnDerivative);
        }
        
    }
}

// Helper to compute layer outputs
std::vector<double> NeuralNetwork::computeLayerOutput(const std::vector<Perceptron>& layer, const std::vector<double>& inputs, bool isOutput) {
    std::vector<double> outputs;
    for (const auto& perceptron : layer) {
        outputs.push_back(perceptron.guess(inputs));
    }
    if (isOutput && outputActivationFn == nullptr) {
        return softmax(outputs);
    }
    return outputs;
}

// Predict output for given inputs
std::vector<double> NeuralNetwork::predict(const std::vector<double>& inputs) {
    if (inputs.size() != inputSize) {
        throw std::invalid_argument("Input size does not match the network's input layer size.");
    }

    std::vector<double> hiddenOutputs;
    if (hiddenSize > 0) {
        hiddenOutputs = computeLayerOutput(hiddenLayer, inputs, false);
    }

    // Pass through the hidden layer


    // Pass through the output layer
    return computeLayerOutput(outputLayer, hiddenSize > 0 ? hiddenOutputs : inputs, true);
}



// Train the neural network
void NeuralNetwork::train(const std::vector<double>& inputs, const std::vector<double>& targets) {
    if (inputs.size() != inputSize || targets.size() != outputSize) {
        throw std::invalid_argument("Input or target size does not match the network's configuration.");
    }

    // Forward pass
    std::vector<double> finalOutputs;
    std::vector<double> hiddenOutputs;
    if (hiddenSize > 0) {
        hiddenOutputs = computeLayerOutput(hiddenLayer, inputs, false);
        finalOutputs = computeLayerOutput(outputLayer, hiddenOutputs, true);
    }
    else {
        finalOutputs = computeLayerOutput(outputLayer, inputs, true);
    }

    // Calculate errors for output layer
    std::vector<double> outputErrors(outputLayer.size());
    for (size_t i = 0; i < outputLayer.size(); ++i) {
        outputErrors[i] = targets[i] - finalOutputs[i];
    }
    // Update output layer weights
    for (size_t i = 0; i < outputLayer.size(); ++i) {
        outputLayer[i].train(hiddenSize > 0 ? hiddenOutputs : inputs, targets[i]);
    }

    // Skip hidden layer weight updates if hiddenSize == 0
    if (hiddenSize > 0) {
        // Calculate errors for hidden layer
        std::vector<double> hiddenErrors(hiddenLayer.size(), 0.0);
        for (size_t i = 0; i < hiddenLayer.size(); ++i) {
            for (size_t j = 0; j < outputLayer.size(); ++j) {
                hiddenErrors[i] += outputErrors[j] * outputLayer[j].guess(hiddenOutputs); // Backpropagate error
            }
        }

        // Update hidden layer weights
        for (size_t i = 0; i < hiddenLayer.size(); ++i) {
            hiddenLayer[i].train(inputs, hiddenErrors[i]);
        }
    }
}



// Fit on a dataset
void NeuralNetwork::fit(const std::vector<std::vector<double>>& trainingData,
    const std::vector<std::vector<double>>& targets,
    int num_epochs) {
    std::vector<double> errorRates; // Uchovava chybovost za poslednych 10 epoch

    for (int epoch = 0; epoch < num_epochs; ++epoch) {
        int correctPredictions = 0; // Pocet spravnych predikcii v aktualnej epoche
        for (size_t i = 0; i < trainingData.size(); ++i) {
            train(trainingData[i], targets[i]);

            // Skontrolujeme predikciu
            //std::vector<double> prediction = predict(trainingData[i]);

            //// Najdeme index s najvyssou pravdepodobnostou pre predikciu a target
            //size_t predictedIndex = std::distance(prediction.begin(), std::max_element(prediction.begin(), prediction.end()));
            //size_t targetIndex = std::distance(targets[i].begin(), std::max_element(targets[i].begin(), targets[i].end()));

            //if (predictedIndex == targetIndex) {
            //    correctPredictions++;
            //}
        }
        ///*if (epoch % 10 == 0) {
        //    printModel();
        //}*/

        //// Vypocitame uspesnost
        //double accuracy = (static_cast<double>(correctPredictions) / trainingData.size()) * 100.0;
        //errorRates.push_back(100.0 - accuracy); // Chybovost = 100% - uspesnost

        //// Uchovavame iba poslednych 10 hodnot
        //if (errorRates.size() > 10) {
        //    errorRates.erase(errorRates.begin());
        //}

        //// Vypis uspesnosti kazdych 10 epoch
        //if ((epoch + 1) % 10 == 0 || epoch == num_epochs - 1) {
        //    double averageErrorRate = std::accumulate(errorRates.begin(), errorRates.end(), 0.0) / errorRates.size();
        //    /*if (averageErrorRate < 5.0) {
        //        for (size_t i = 0; i < trainingData.size(); ++i) {
        //            std::vector<double> prediction = predict(trainingData[i]);
        //            std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << "\n" << " Target: " << targets[i] << "\n";
        //            int predictedClass = std::distance(prediction.begin(), std::max_element(prediction.begin(), prediction.end()));
        //            std::cout << "Predicted Class: " << predictedClass << "\n";
        //        }
        //    }*/
        //    std::cout << "Epoch " << epoch + 1 << ": Average Error Rate (last 10 epochs): " << averageErrorRate << "%" << std::endl;
        //}
    }
}





// Set activation function for all perceptrons in the hidden layer
void NeuralNetwork::setHiddenActivationFunction(std::function<double(double)> activation,
    std::function<double(double)> activationDerivative) {
    for (auto& perceptron : hiddenLayer) {
        perceptron.setActivationFunction(activation, activationDerivative);
    }

}
// Set activation function for all perceptrons in the output layer
void NeuralNetwork::setOutputActivationFunction(std::function<double(double)> activation,
    std::function<double(double)> activationDerivative) {
    for (auto& perceptron : outputLayer) {
        perceptron.setActivationFunction(activation, activationDerivative);
    }
}

// Print model information
void NeuralNetwork::printModel() const {
    std::cout << "Hidden Layer:" << std::endl;
    for (size_t i = 0; i < hiddenLayer.size(); ++i) {
        std::cout << "Perceptron " << i + 1 << ": ";
        hiddenLayer[i].printModel();
    }

    std::cout << "Output Layer:" << std::endl;
    for (size_t i = 0; i < outputLayer.size(); ++i) {
        std::cout << "Perceptron " << i + 1 << ": ";
        outputLayer[i].printModel();
    }
}

// Get the bias (assume all perceptrons share the same bias for simplicity)
double NeuralNetwork::getBias() const {
    return bias;
}

// Set the bias for all perceptrons in the network
void NeuralNetwork::setBias(double newBias) {
    bias = newBias;
    for (auto& perceptron : hiddenLayer) {
        perceptron.setBias(newBias);
    }
    for (auto& perceptron : outputLayer) {
        perceptron.setBias(newBias);
    }
}

// Get the learning rate (assume all perceptrons share the same learning rate for simplicity)
double NeuralNetwork::getLearningRate() const {
    return learningRate;
}

// Set the learning rate for all perceptrons in the network
void NeuralNetwork::setLearningRate(double newLearningRate) {
    learningRate = newLearningRate;
    for (auto& perceptron : hiddenLayer) {
        perceptron.setLearningRate(newLearningRate);
    }
    for (auto& perceptron : outputLayer) {
        perceptron.setLearningRate(newLearningRate);
    }
}


using json = nlohmann::json;

void NeuralNetwork::saveNetwork(const std::string& filename) {
    json networkJson;

    // Add general network properties
    networkJson["activationFunction"] = "ReLu"; // You can set this dynamically if needed
    networkJson["learningRate"] = learningRate;
    networkJson["bias"] = bias;
    networkJson["inputSize"] = inputSize;
    networkJson["hiddenSize"] = hiddenSize;
    networkJson["outputSize"] = outputSize;

    // Add layers
    networkJson["layers"] = json::array();

    // Save hidden layer weights
    json hiddenLayerJson = json::array();
    for (const auto& perceptron : hiddenLayer) {
        json perceptronJson;
        perceptronJson["weights"] = perceptron.getWeights(); // Implement getWeights() in Perceptron
        hiddenLayerJson.push_back(perceptronJson);
    }
    networkJson["layers"].push_back(hiddenLayerJson);

    // Save output layer weights
    json outputLayerJson = json::array();
    for (const auto& perceptron : outputLayer) {
        json perceptronJson;
        perceptronJson["weights"] = perceptron.getWeights(); // Implement getWeights() in Perceptron
        outputLayerJson.push_back(perceptronJson);
    }
    networkJson["layers"].push_back(outputLayerJson);

    // Save to file
    std::ofstream file(filename);
    if (file.is_open()) {
        file << networkJson.dump(4); // Pretty print with 4 spaces
        file.close();
        std::cout << "Neural network saved to " << filename << std::endl;
    }
    else {
        std::cerr << "Failed to open file for writing!" << std::endl;
    }
}

void NeuralNetwork::loadNetwork(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open the file: " + filename);
    }

    json networkJson;
    file >> networkJson; // Parse the JSON file
    file.close();

    // Validate the JSON structure
    if (!networkJson.contains("learningRate") ||
        !networkJson.contains("bias") ||
        !networkJson.contains("layers")) {
        throw std::runtime_error("Invalid JSON format for NeuralNetwork.");
    }

    // Update learningRate and bias
    learningRate = networkJson["learningRate"].get<double>();
    bias = networkJson["bias"].get<double>();

    // Update weights for hidden layer
    const auto& hiddenLayerJson = networkJson["layers"][0];
    if (hiddenLayerJson.size() != hiddenLayer.size()) {
        throw std::runtime_error("Mismatch in hidden layer size.");
    }
    for (size_t i = 0; i < hiddenLayer.size(); ++i) {
        hiddenLayer[i].setWeights(hiddenLayerJson[i]["weights"].get<std::vector<double>>());
    }

    // Update weights for output layer
    const auto& outputLayerJson = networkJson["layers"][1];
    if (outputLayerJson.size() != outputLayer.size()) {
        throw std::runtime_error("Mismatch in output layer size.");
    }
    for (size_t i = 0; i < outputLayer.size(); ++i) {
        outputLayer[i].setWeights(outputLayerJson[i]["weights"].get<std::vector<double>>());
    }

    std::cout << "Neural network successfully loaded from " << filename << std::endl;
}

std::vector<double> NeuralNetwork::softmax(const std::vector<double>& logits) {
    std::vector<double> probabilities(logits.size());
    double maxLogit = *std::max_element(logits.begin(), logits.end()); // Pre numerickú stabilitu
    double sum = 0.0;

    for (size_t i = 0; i < logits.size(); ++i) {
        probabilities[i] = std::exp(logits[i] - maxLogit);
        sum += probabilities[i];
    }
    for (size_t i = 0; i < probabilities.size(); ++i) {
        probabilities[i] /= sum;
    }
    return probabilities;
}
