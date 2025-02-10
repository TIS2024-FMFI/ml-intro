#pragma once
#include <memory>

class Scenario;

class ScenarioFactory {
public:
    static std::unique_ptr<Scenario> create(int scenarioID);
};