#pragma once

#include <vector>
#include <random>
#include <cmath>
#include <algorithm>




std::vector<std::vector<double>> generateTrainingSet1(int n);
std::vector<std::vector<double>> generateTargets1(std::vector<std::vector<double>> trainingSet);
std::vector<std::vector<double>> generateTrainingSet2(int n);
std::vector<std::vector<double>> generateTargets2(std::vector<std::vector<double>> trainingSet);
std::vector<std::vector<double>> generateTrainingSet3(int n);
std::vector<std::vector<double>> generateTargets3(std::vector<std::vector<double>> trainingSet);

double calculateDistance(const std::vector<double>& a, const std::vector<double>& b);
std::vector<double> determineTarget(const std::vector<double>& input, std::vector<std::vector<double>> colorTargets);
