#include "FileDialog.h"

  // For std::runtime_error


std::string openFileDialog() {
    wchar_t filename[MAX_PATH] = L""; // Wide-character buffer

    // Initialize the OPENFILENAME structure
    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = nullptr; // No owner window
    ofn.lpstrFilter = L"JSON Files\0*.json\0All Files\0*.*\0"; // File filters
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

    // Show the Open File dialog
    if (GetOpenFileName(&ofn)) {
        // Convert the wide-character string to a standard string
        std::wstring wideFilename(filename);
        return std::string(wideFilename.begin(), wideFilename.end());
    }
    else {
        throw std::runtime_error("No file selected or dialog canceled.");
    }
}

std::string saveFileDialog() {
    wchar_t filename[MAX_PATH] = L"";
    OPENFILENAME ofn = {};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFilter = L"JSON Files\0*.json\0All Files\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn)) {
        std::wstring wideFilename(filename);
        return std::string(wideFilename.begin(), wideFilename.end());
    }
    else {
        throw std::runtime_error("Save operation canceled.");
    }
}
