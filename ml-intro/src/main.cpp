#include <glad/glad.h>
#include <Windows.h>
#include <tchar.h>
#include "AppManager.h"



int main(void)
{
    AppManager app = AppManager();

    // TESTING NN SCENAR1
    /*NeuralNetwork nn(2, 0, 1,nullptr, nullptr, [](double x) { return Sigmoid().function(x); }, [](double x) { return Sigmoid().derivative(x); });;

    std::vector<std::vector<double>> targets = {
    {1}, 
    {1}, 
    {1}, 
    {0}, 
    {0}, 
    {0}, 
    };
    std::vector<std::vector<double>> trainingData = {
    {1.0, 0.0}, 
    {0.8, 0.1}, 
    {0.9, 0.2}, 
    {0.0, 1.0}, 
    {0.1, 0.8}, 
    {0.2, 0.9}, 
    };
    std::cout << "Predictions before training:\n";
    for (size_t i = 0; i < trainingData.size(); ++i) {
        std::vector<double> prediction = nn.predict(trainingData[i]);
        std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << " Target: " << targets[i] << "\n";
    }
    nn.fit(trainingData, targets, 100);

    std::cout << "Predictions After training:\n";
    for (size_t i = 0; i < trainingData.size(); ++i) {
        std::vector<double> prediction = nn.predict(trainingData[i]);
        std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << " Target: " << targets[i] << "\n";
    }*/

    //TESTING NN SCENAR2
//    NeuralNetwork nn(3, 5, 7, [](double x) { return ReLu().function(x); }, [](double x) { return ReLu().derivative(x); }, nullptr, nullptr);
//    std::vector<std::vector<double>> trainingData = {
//    {1.0, 0.0, 0.0}, // Cervena
//    {1.0, 0.0, 1.0}, // Fialova
//    {1.0, 1.0, 0.0}, // Zlta
//    {1.0, 1.0, 1.0}, // Biela
//    {0.0, 0.0, 1.0}, // Modra
//    {0.0, 1.0, 1.0}, // Azurova
//    {0.0, 1.0, 0.0}, // Zelena
//    };
//    std::vector<std::vector<double>> targets = {
//        {1, 0, 0, 0, 0, 0, 0}, // Cervena
//        {0, 1, 0, 0, 0, 0, 0}, // Fialova
//        {0, 0, 1, 0, 0, 0, 0}, // Zlta
//        {0, 0, 0, 1, 0, 0, 0}, // Biela
//        {0, 0, 0, 0, 1, 0, 0}, // Modra
//        {0, 0, 0, 0, 0, 1, 0}, // Azurova
//        {0, 0, 0, 0, 0, 0, 1}, // Zelena
//    };
//    std::cout << "==============================================================" << endl;
//    std::cout << "Predictions before training:\n";
//    for (size_t i = 0; i < trainingData.size(); ++i) {
//        std::vector<double> prediction = nn.predict(trainingData[i]);
//        std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << "\n" << " Target: " << targets[i] << "\n";
//        int predictedClass = std::distance(prediction.begin(), std::max_element(prediction.begin(), prediction.end()));
//        std::cout << "Predicted Class: " << predictedClass << "\n";
//    }
//    nn.fit(trainingData, targets, 500);
//    std::cout << "==============================================================" << endl;
//    std::cout << "Predictions After training500:\n";
//    for (size_t i = 0; i < trainingData.size(); ++i) {
//        std::vector<double> prediction = nn.predict(trainingData[i]);
//        std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << "\n" << " Target: " << targets[i] << "\n";
//        int predictedClass = std::distance(prediction.begin(), std::max_element(prediction.begin(), prediction.end()));
//        std::cout << "Predicted Class: " << predictedClass << "\n";
//    }
//    std::cout << "==============================================================" << endl;
//    nn.fit(trainingData, targets, 500);
//    for (size_t i = 0; i < trainingData.size(); ++i) {
//        std::vector<double> prediction = nn.predict(trainingData[i]);
//        std::cout << "Input: " << trainingData[i] << " Prediction: " << prediction << "\n" << " Target: " << targets[i] << "\n";
//        int predictedClass = std::distance(prediction.begin(), std::max_element(prediction.begin(), prediction.end()));
//        std::cout << "Predicted Class: " << predictedClass << "\n";
//    }
}