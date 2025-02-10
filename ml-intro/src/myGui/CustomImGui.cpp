#include "CustomImGui.h"


bool GradientColorPicker(vector<float>& myVector) {
    bool isPressed = false;
    ImVec2 gradient_size(200, 200);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
    ImVec2 gradient_end(cursor_pos.x + gradient_size.x, cursor_pos.y + gradient_size.y);

    draw_list->AddRectFilledMultiColor(
        cursor_pos, gradient_end,
        IM_COL32(0, 0, 0, 255),
        IM_COL32(255, 0, 0, 255),
        IM_COL32(255, 255, 0, 255),
        IM_COL32(0, 255, 0, 255)
    );

    // Capture mouse interaction
    ImGui::InvisibleButton("##GradientPicker", gradient_size);
    if (ImGui::IsItemActive()) {
        ImVec2 mouse_pos = ImGui::GetMousePos();
        float x_ratio = (mouse_pos.x - cursor_pos.x) / gradient_size.x;
        float y_ratio = (mouse_pos.y - cursor_pos.y) / gradient_size.y;

        x_ratio = x_ratio < 0 ? 0.0f : (x_ratio > 1 ? 1.0f : x_ratio); //camp(x,0,1)
        y_ratio = y_ratio < 0 ? 0.0f : (y_ratio > 1 ? 1.0f : y_ratio);

        myVector[0] = x_ratio;      //r
        myVector[1] = y_ratio;      //g

        isPressed = true;
    }

    // Highlight the current color position on the gradient
    float marker_x = cursor_pos.x + gradient_size.x * myVector[0];
    float marker_y = cursor_pos.y + gradient_size.y * myVector[1];
    draw_list->AddCircle(ImVec2(marker_x, marker_y), 5.0f, IM_COL32(255, 255, 255, 255), 12, 2.0f);
    return isPressed;
}



void OuputSelection(int output, vector<pair<ImColor, string>>& colors) {
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 p = ImGui::GetCursorScreenPos();

    float x = p.x + 4.0f;
    float y = p.y + 4.0f;
    const float r = 20.0f;
    const float spacing = 10.0f;
    ImVec2 mid = ImVec2(x + r, y + r);

    for (size_t i = 0; i < colors.size(); ++i) {
        ImVec4 color = colors[i].first;

        if (i == output) {
            ImVec4 darker = ImVec4(color.x * 0.7f, color.y * 0.7f, color.z * 0.7f, 1.0f);
            draw_list->AddCircle(mid, r * 1.2f, ImColor(darker), 0, 3.0f);
        }

        draw_list->AddCircleFilled(mid, r, ImColor(color));
        mid.x += spacing + 2 * r;
    }


    ImGui::Dummy(ImVec2((r*2 + spacing), (r*2 + spacing)));
}

bool myColoredButton(string text, ImVec4 color) {
    ImVec4 hover = ImVec4(color.x * .7f, color.y * .7f, color.z * .7f, 1.0f);
    ImVec4 click = ImVec4(color.x * .4f, color.y * .4f, color.z * .4f, 1.0f);

    float luminance = (0.299f * color.x + 0.587f * color.y + 0.114f * color.z);
    ImGui::PushStyleColor(ImGuiCol_Button, color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, hover);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, click);
    ImGui::PushStyleColor(ImGuiCol_Text, (luminance > 0.5f) ? ImVec4(0, 0, 0, 1.0f) : ImVec4(1, 1, 1, 1.0f));

    ImVec2 button_size = ImVec2(60, 30);
    bool out = ImGui::Button(text.c_str(), button_size);

    ImGui::PopStyleColor(4);

    return out;
}

void myMultiSelector(vector<string> options, int& selectedOption) {
    for (int i = 0; i < options.size(); i++) {
        bool isSelected = (selectedOption == i);
        if (ImGui::Selectable(options[i].c_str(), isSelected, ImGuiSelectableFlags_DontClosePopups)) {
            selectedOption = i;
        }
    }
}

bool isDrawing = false;
bool myBitmapEditor(ImVec2 canvasSize, ImVec2 gridSize, vector<float>& NetworkInputVector) {
    ImVec2 squareSize = ImVec2(canvasSize.x / gridSize.x, canvasSize.y / gridSize.y);
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    ImVec2 mousePos = ImGui::GetMousePos();

    for (int y = 0; y < gridSize.y; y++) {
        for (int x = 0; x < gridSize.x; x++) {
            ImVec2 topLeft = ImVec2(canvasPos.x + x * squareSize.x, canvasPos.y + y * squareSize.y);
            ImVec2 bottomRight = ImVec2(topLeft.x + squareSize.x, topLeft.y + squareSize.y);

            auto col = NetworkInputVector[y * gridSize.x + x] < 0.5 ? IM_COL32(255, 255, 255, 255): IM_COL32(0, 0, 0, 255);
            drawList->AddRectFilled(topLeft, bottomRight, col);

            // Draw the grid
            drawList->AddRect(topLeft, bottomRight, IM_COL32(200, 200, 200, 255));

            bool isMouseInside = (mousePos.x >= topLeft.x && mousePos.x < bottomRight.x &&
                                  mousePos.y >= topLeft.y && mousePos.y < bottomRight.y);

            if (isMouseInside) { //ImGui::IsItemHoverd() nefunguje ked je lave tlacidlo mysi stlacene
                if (ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
                    NetworkInputVector[y * gridSize.x + x] = 1;
                    isDrawing = true;
                }
                if (ImGui::IsMouseDown(ImGuiMouseButton_Right)) {
                    NetworkInputVector[y * gridSize.x + x] = 0;
                    isDrawing = true;
                }
            }
        }
    }
    ImGui::SetCursorScreenPos(ImVec2(canvasPos.x, canvasPos.y + canvasSize.y));
    if (isDrawing && (ImGui::IsMouseReleased(ImGuiMouseButton_Left) || ImGui::IsMouseReleased(ImGuiMouseButton_Right))) {
        isDrawing = false;
        return true;
    }
    return false;
}