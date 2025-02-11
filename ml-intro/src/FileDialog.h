#pragma once

#include "string"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commdlg.h>
#include <stdexcept>

std::string openFileDialog();
std::string saveFileDialog();