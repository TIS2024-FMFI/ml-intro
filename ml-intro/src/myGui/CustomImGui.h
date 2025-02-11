#pragma once
#include "imgui.h"
#include <vector>
#include <string>

using namespace std;

bool GradientColorPicker(vector<float>& myVector);
void OuputSelection(int output, vector<pair<ImColor, string>>& colors);
bool myColoredButton(string text, ImVec4 color);
void myMultiSelector(vector<string> options, int& selectedOption);
bool myBitmapEditor(ImVec2 canvasSize, ImVec2 gridSize, vector<float>& NetworkInputVector);