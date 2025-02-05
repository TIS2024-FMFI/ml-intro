#include "trainingSet.h"

std::vector<Eigen::VectorXd> generateTrainingSet1(int n) {
    std::vector<Eigen::VectorXd> trainingSet;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    trainingSet.push_back((Eigen::VectorXd(2) << 1.0, 0.0).finished());
    trainingSet.push_back((Eigen::VectorXd(2) << 0.0, 1.0).finished());

    for (int i = 2; i < n; ++i) {
        trainingSet.emplace_back(Eigen::VectorXd::Random(2).unaryExpr([&dist, &gen](double) { return dist(gen); }));
    }
    return trainingSet;
}

std::vector<Eigen::VectorXd> generateTargets1(const std::vector<Eigen::VectorXd>& trainingSet) {
    std::vector<Eigen::VectorXd> targets;
    for (const auto& point : trainingSet) {
        if (point[0] > point[1]) {
            targets.emplace_back(Eigen::VectorXd::Constant(1, 0.0));
        }
        else {
            targets.emplace_back(Eigen::VectorXd::Constant(1, 1.0));
        }
    }
    return targets;
}

// Training set for multi-class classification (3D inputs)
std::vector<Eigen::VectorXd> generateTrainingSet2(int n) {
    std::vector<Eigen::VectorXd> trainingSet;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    std::vector<Eigen::VectorXd> colorTargets = {
        (Eigen::VectorXd(3) << 1.0, 0.0, 0.0).finished(),
        (Eigen::VectorXd(3) << 1.0, 0.0, 1.0).finished(),
        (Eigen::VectorXd(3) << 1.0, 1.0, 0.0).finished(),
        (Eigen::VectorXd(3) << 1.0, 1.0, 1.0).finished(),
        (Eigen::VectorXd(3) << 0.0, 0.0, 1.0).finished(),
        (Eigen::VectorXd(3) << 0.0, 1.0, 1.0).finished(),
        (Eigen::VectorXd(3) << 0.0, 1.0, 0.0).finished()
    };

    for (const auto& color : colorTargets) {
        trainingSet.push_back(color);
    }

    for (int i = colorTargets.size(); i < n; ++i) {
        trainingSet.emplace_back(Eigen::VectorXd::Random(3).unaryExpr([&dist, &gen](double) { return dist(gen); }));
    }

    return trainingSet;
}


std::vector<Eigen::VectorXd> generateTargets2(const std::vector<Eigen::VectorXd>& trainingSet) {
    std::vector<Eigen::VectorXd> colorTargets = {
        (Eigen::VectorXd(3) << 1.0, 0.0, 0.0).finished(),
        (Eigen::VectorXd(3) << 1.0, 0.0, 1.0).finished(),
        (Eigen::VectorXd(3) << 1.0, 1.0, 0.0).finished(),
        (Eigen::VectorXd(3) << 1.0, 1.0, 1.0).finished(),
        (Eigen::VectorXd(3) << 0.0, 0.0, 1.0).finished(),
        (Eigen::VectorXd(3) << 0.0, 1.0, 1.0).finished(),
        (Eigen::VectorXd(3) << 0.0, 1.0, 0.0).finished()
    };

    std::vector<Eigen::VectorXd> targets;
    for (const auto& point : trainingSet) {
        double minDistance = std::numeric_limits<double>::max();
        int closestColorIndex = -1;

        for (size_t i = 0; i < colorTargets.size(); ++i) {
            double distance = (point - colorTargets[i]).squaredNorm();
            if (distance < minDistance) {
                minDistance = distance;
                closestColorIndex = i;
            }
        }

        Eigen::VectorXd oneHot = Eigen::VectorXd::Zero(colorTargets.size());
        oneHot[closestColorIndex] = 1.0;
        targets.emplace_back(oneHot);
    }
    return targets;
}




std::pair<std::vector<Eigen::VectorXd>, std::vector<Eigen::VectorXd>> loadFixedTrainingSet(int limit) {
    std::vector<Eigen::VectorXd> trainingData;
    std::vector<Eigen::VectorXd> targets;

    std::ifstream file("resources/mnist_train.csv");
    if (!file) {
        throw std::runtime_error("Unable to open file: mnist_train.csv");
    }

    std::string line;

    // Generate a random number of lines to skip
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 499 - limit);
    int linesToSkip = dis(gen);

    // Skip the random number of lines
    while (linesToSkip-- > 0 && std::getline(file, line)) {
        // Simply iterate through lines without processing
    }

    int loadedCases = 0;

    // Read each line from the CSV and load the data
    while (std::getline(file, line) && loadedCases < limit) {
        std::stringstream ss(line);

        Eigen::VectorXd input(28 * 28); // 784 pixels for a 28x28 image
        Eigen::VectorXd target(10);    // One-hot target for digits 0-9

        std::string value;
        int index = 0;

        // Read the label (first value)
        std::getline(ss, value, ',');
        int label = std::stoi(value);
        target.setZero();
        target[label] = 1.0;

        // Read the 784 pixel values
        while (std::getline(ss, value, ',')) {
            input[index++] = std::stod(value) / 255.0; // Normalize pixel values to 0-1
        }

        trainingData.push_back(input);
        targets.push_back(target);

        ++loadedCases;
    }

    file.close();
    return { trainingData, targets };
}





