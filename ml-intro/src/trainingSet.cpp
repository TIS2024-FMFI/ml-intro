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




