#include "functions.h"
#include <string>
#include <iostream>
#include <filesystem>
#include <cstdlib>

void displayImage(const std::string& imagePath) {
    if (!std::filesystem::exists(imagePath)) {
        std::cerr << "Error: Image does not exist.\n";
        return;
    }

    // Open the image with the default viewer
    #ifdef _WIN32
        system(("start " + imagePath).c_str());
    #elif __APPLE__
        system(("open " + imagePath).c_str());
    #else
        system(("xdg-open " + imagePath).c_str());
    #endif
    
}
