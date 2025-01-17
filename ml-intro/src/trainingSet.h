#pragma once

#include <Eigen/Dense>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>




std::vector<Eigen::VectorXd> generateTrainingSet1(int n);
std::vector<Eigen::VectorXd> generateTargets1(const std::vector<Eigen::VectorXd>& trainingSet);
std::vector<Eigen::VectorXd> generateTrainingSet2(int n);
std::vector<Eigen::VectorXd> generateTargets2(const std::vector<Eigen::VectorXd>& trainingSet);
std::vector<std::vector<double>> generateTrainingSet3(int n);
std::vector<std::vector<double>> generateTargets3(const std::vector<Eigen::VectorXd>& trainingSet);

