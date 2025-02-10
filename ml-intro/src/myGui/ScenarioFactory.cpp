#include "ScenarioFactory.h"
#include "Scenario.h"
#include "Scenario_1.h"
#include "Scenario_2.h"
#include "Scenario_3.h"

std::unique_ptr<Scenario> ScenarioFactory::create(int scenarioID) {
    switch (scenarioID) {
    case 1: return std::make_unique<Scenario_1>();
    case 2: return std::make_unique<Scenario_2>();
    case 3: return std::make_unique<Scenario_3>();
    default: return nullptr;
    }
}