#pragma once
#include "imgui.h"

void SetSoftLightTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
    colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.75f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.92f, 0.96f, 0.10f);

    colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.90f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75f, 0.75f, 0.80f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.70f, 0.70f, 0.75f, 1.00f);

    colors[ImGuiCol_TitleBg] = ImVec4(0.72f, 0.72f, 0.75f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.65f, 0.65f, 0.74f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.80f, 0.80f, 0.85f, 1.00f);

    colors[ImGuiCol_MenuBarBg] = ImVec4(0.88f, 0.88f, 0.92f, 1.00f);

    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.92f, 0.92f, 0.96f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.70f, 0.70f, 0.75f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.60f, 0.60f, 0.65f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50f, 0.50f, 0.55f, 1.00f);

    colors[ImGuiCol_Button] = ImVec4(0.75f, 0.75f, 0.85f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.65f, 0.65f, 0.75f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.55f, 0.55f, 0.65f, 1.00f);

    colors[ImGuiCol_CheckMark] = ImVec4(0.20f, 0.40f, 0.80f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.50f, 0.80f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 0.45f, 0.75f, 1.00f);

    colors[ImGuiCol_Header] = ImVec4(0.70f, 0.75f, 0.85f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.60f, 0.65f, 0.75f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.55f, 0.60f, 0.70f, 1.00f);

    colors[ImGuiCol_Separator] = ImVec4(0.75f, 0.75f, 0.80f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.70f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.55f, 0.55f, 0.65f, 1.00f);

    colors[ImGuiCol_ResizeGrip] = ImVec4(0.60f, 0.60f, 0.70f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.60f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.40f, 0.50f, 1.00f);

    colors[ImGuiCol_Tab] = ImVec4(0.80f, 0.80f, 0.85f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.70f, 0.70f, 0.75f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.65f, 0.65f, 0.70f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.85f, 0.85f, 0.90f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.75f, 0.75f, 0.80f, 1.00f);

    style.FrameRounding = 6.0f;
    style.GrabRounding = 6.0f;
    style.WindowRounding = 8.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 8.0f;
    style.TabRounding = 6.0f;
}