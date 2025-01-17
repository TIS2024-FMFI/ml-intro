#include "trainingSet.h"

std::vector<std::vector<double>> generateTrainingSet1(int n) {
    std::vector<std::vector<double>> trainingSet;

    // Random number generator for values in the range [0, 1.0]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    // Generate 'n' random 2D points
    for (int i = 0; i < n; ++i) {
        trainingSet.push_back({ dist(gen), dist(gen) });
    }

    return trainingSet;
}

std::vector<std::vector<double>> generateTargets1(std::vector<std::vector<double>> trainingSet) {
    std::vector<std::vector<double>> targets;

    for (const auto& dataPoint : trainingSet) {
        if (dataPoint[0] > dataPoint[1]) {
            targets.push_back({ 1.0 }); // First value is larger
        }
        else {
            targets.push_back({ 0.0 }); // Second value is larger
        }
    }

    return targets;
}

std::vector<std::vector<double>> generateTrainingSet2(int n) {
    std::vector<std::vector<double>> trainingSet;

    // Random number generator for values in the range [0, 1.0]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    // Generate 'n' random 3D points
    for (int i = 0; i < n; ++i) {
        trainingSet.push_back({ dist(gen), dist(gen), dist(gen)});
    }

    return trainingSet;
}


std::vector<std::vector<double>> generateTargets2(std::vector<std::vector<double>> trainingSet) {
    std::vector<std::vector<double>> colorTargets = {
    {1.0, 0.0, 0.0}, // Cervena
    {1.0, 0.0, 1.0}, // Fialova
    {1.0, 1.0, 0.0}, // Zlta
    {1.0, 1.0, 1.0}, // Biela
    {0.0, 0.0, 1.0}, // Modra
    {0.0, 1.0, 1.0}, // Azurova
    {0.0, 1.0, 0.0}  // Zelena
    };
    std::vector<std::vector<double>> targets;

    for (const auto& dataPoint : trainingSet) {
        targets.push_back(determineTarget(dataPoint, colorTargets));
    }
    return targets;
}



double calculateDistance(const std::vector<double>& a, const std::vector<double>& b) {
    double sum = 0.0;
    for (size_t i = 0; i < a.size(); ++i) {
        sum += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return std::sqrt(sum);
}


std::vector<double> determineTarget(const std::vector<double>& input, std::vector<std::vector<double>> colorTargets) {
    double minDistance = std::numeric_limits<double>::max();
    int closestColorIndex = -1;

    // Porovnanie vstupu s každým ideálnym vektorom
    for (size_t i = 0; i < colorTargets.size(); ++i) {
        double distance = calculateDistance(input, colorTargets[i]);
        if (distance < minDistance) {
            minDistance = distance;
            closestColorIndex = i;
        }
    }

    // Generovanie targetu pre najbližšiu farbu
    std::vector<double> target(colorTargets.size(), 0.0);
    target[closestColorIndex] = 1.0;
    return target;
}


