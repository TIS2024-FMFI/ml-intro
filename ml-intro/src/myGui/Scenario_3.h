#pragma once
#include "Scenario.h"

class Scenario_3 : public Scenario {
public:
    bool customInputBlock(string name, vector<float>& networkInput) override {
        bool isChanged = false;
        InputCamera::CloseCamera();
        if (ImGui::CollapsingHeader(name.c_str())) {
            isChanged = myBitmapEditor(ImVec2(10 * 28, 10 * 28), ImVec2(28, 28), networkInput);
            if (ImGui::Button("Clear")) {
                networkInput = std::vector<float>(networkInput.size(), 0);
                isChanged = true;
            }
        }
        return isChanged;
    }

    void customOutputBlock(string name, int& output) override {
        if (ImGui::CollapsingHeader(name.c_str())) {
            ImGui::Text("Result: %d", output);
        }
    }

    int customPickOutputBlock(string name) override {
        int out = -1;
        if (ImGui::CollapsingHeader(name.c_str())) {
            for (size_t i = 0; i < 10; i++)
            {
                if (ImGui::Button(std::to_string(i).c_str())) {
                    //todo: make this make sense . . . AppManager::getInstance().tellOutput(i);
                    out = i;
                }
                if (i < 9) {
                    ImGui::SameLine();
                }
            }
        }
        return out;
    }





private:

    vector<pair<ImColor, string>> outputColors = {
        { ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "red"},
        { ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "green"},
    };
};