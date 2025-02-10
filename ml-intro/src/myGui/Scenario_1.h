#pragma once
#include "Scenario.h"

class Scenario_1 : public Scenario {
public:
    bool customInputBlock(string name, vector<float>& networkInput) override {
        bool wasChanged = false;
        if (ImGui::CollapsingHeader(name.c_str())) {
            wasChanged = GradientColorPicker(networkInput);
            ImGui::SameLine();
            ImGui::Text("Selected Color: ");
            ImGui::SameLine();
            ImGui::ColorButton("##Preview", ImVec4(networkInput[0], networkInput[1], 0.0f, 1.0f));
        }
        return wasChanged;
    }

    void customOutputBlock(string name, int& output) override {
        if (ImGui::CollapsingHeader(name.c_str())) {
            OuputSelection(output, outputColors);
        }
    }

    int customPickOutputBlock(string name) override {
        int clicked = -1;
        if (ImGui::CollapsingHeader(name.c_str())) {
            for (int i = 0; i < outputColors.size(); i++) {
                if (myColoredButton(outputColors[i].second, outputColors[i].first)) {
                    clicked = i; // todo AppManager::GetInstance().tellOutput(i) a zmen navratovu hodnotu metody na void
                }
                if (i != outputColors.size() - 1) {
                    ImGui::SameLine();
                }
            }
        }
        return clicked;
    }





private:

    vector<pair<ImColor, string>> outputColors = {
        { ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "red"},
        { ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "green"},
    };
};