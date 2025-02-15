#pragma once
#include "Scenario.h"

class Scenario_2 : public Scenario {
public:
    bool customInputBlock(string name, vector<float>& networkInput) override {
        if (ImGui::CollapsingHeader((name).c_str())) {
            static bool useCamera = false;
            if (ImGui::Button(useCamera ? "Simulate Input" : "Camera Input", ImVec2(150, 20))) {
                useCamera = !useCamera;
                InputCamera::SetDefault();
            }

            if (useCamera) {
                auto camOutput = InputCamera::CameraSelectorUI();
                ImGui::Text("Selected Color: ");
                ImGui::SameLine();
                if (camOutput.size() == 0) {
                    ImGui::ColorButton("##Preview", ImVec4(networkInput[0], networkInput[1], networkInput[2], 1.0f));
                    return false;
                }
                networkInput = camOutput;
                ImGui::ColorButton("##Preview", ImVec4(networkInput[0], networkInput[1], networkInput[2], 1.0f));
                return true;
            }
            else {
                InputCamera::CloseCamera();
                return ImGui::ColorEdit3((name + "##vec3").c_str(), networkInput.data());
            }
        }
        return false;
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
        { ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "magenta"},
        { ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "yellow"},
        { ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "white"},
        { ImVec4(0.0f, 0.0f, 1.0f, 1.0f), "blue"},
        { ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "cyan"},
        { ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "green"}
    };
};