#pragma once
#include "Scenario.h"

class Scenario_1 : public Scenario {
public:
    bool customInputBlock(string name, vector<float>& networkInput) override {
        bool wasChanged = false;
        if (ImGui::CollapsingHeader(name.c_str())) {
            static bool useCamera = false;
            if (ImGui::Button(useCamera ? "Simulate Input" : "Camera Input", ImVec2(150, 25))) {
                useCamera = !useCamera;
                InputCamera::SetDefault();
            }
            if (useCamera) {
                auto camOutput = InputCamera::CameraSelectorUI();
                if (camOutput.size() != 0) {
                    wasChanged = true;
                    networkInput = { camOutput[0], camOutput[1] };
                }
                ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                ImGui::Text("This Network supports only vec2 input.");
                ImGui::Text("blue channel is ignored!");
                ImGui::PopStyleColor();
            }
            else {
                InputCamera::CloseCamera();
                wasChanged = GradientColorPicker(networkInput);
                ImGui::SameLine();
            }
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