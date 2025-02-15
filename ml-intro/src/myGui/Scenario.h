#pragma once
#include "../ImGuiApp.h"
#include "CustomImGui.h"
#include <iostream>
#include <ranges>
#include "InputCamera.h"

class Scenario {
public:
    virtual bool customInputBlock(string name, vector<float>& networkInput) = 0;
    virtual void customOutputBlock(string name, int& output) = 0;
    virtual int  customPickOutputBlock(string name) = 0;
    virtual ~Scenario() = default;

    bool customFloatSlider(string name, float& value, float min, float max) {
        if (ImGui::CollapsingHeader(name.c_str())) {
            return ImGui::SliderFloat((name + "##slider").c_str(), &value, min, max);
        }
    }

    bool customDoubleSlider(string name, double& value, double min, double max) {
        if (ImGui::CollapsingHeader(name.c_str())) {
            float tempFloat = static_cast<float>(value);
            if (ImGui::SliderFloat((name + "##slider").c_str(), &tempFloat, min, max)) {
                value = static_cast<double>(tempFloat);
                return true;
            }
        }
        return false;
    }

    bool customIntSlider(string name, int& value, int min, int max) {
        if (ImGui::CollapsingHeader(name.c_str())) {
            return ImGui::SliderInt((name + "##slider").c_str(), &value, min, max);
        }
    }

    void customMultiSelector(string name, vector<string>& options, int& selectedOption) {
        ImGui::PushID(name.c_str());
        if (ImGui::CollapsingHeader(name.c_str())) {
            myMultiSelector(options, selectedOption);
        }
        ImGui::PopID();
    }

    int output = 0;
private:
};