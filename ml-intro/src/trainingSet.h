#pragma once

#include <Eigen/Dense>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>



std::vector<Eigen::VectorXd> generateTrainingSet1(int n);
std::vector<Eigen::VectorXd> generateTargets1(const std::vector<Eigen::VectorXd>& trainingSet);
std::vector<Eigen::VectorXd> generateTrainingSet2(int n);
std::vector<Eigen::VectorXd> generateTargets2(const std::vector<Eigen::VectorXd>& trainingSet);
std::pair<std::vector<Eigen::VectorXd>, std::vector<Eigen::VectorXd>> loadFixedTrainingSet(int limit = 50);

