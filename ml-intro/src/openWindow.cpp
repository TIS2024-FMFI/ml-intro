#include "openWindow.h"
#include <iostream>
#include <cstdlib>

int openWindow(const std::string& filePath= "resources/example.html") {
#ifdef _WIN32
    std::string command = "start " + filePath;
#endif

    int result = std::system(command.c_str());

    if (result == 0) {
        std::cout << "HTML file opened successfully!" << std::endl;
    }
    else {
        std::cerr << "Failed to open the HTML file." << std::endl;
    }

    return 0;
}


