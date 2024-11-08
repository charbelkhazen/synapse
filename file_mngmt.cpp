#include <vector>
#include <string>
#include <filesystem>
#include <iostream>  // Added this line

std::vector<std::string> getJpgFiles(const std::string& directory) {
    std::vector<std::string> jpgFiles;
    
    if (!std::filesystem::exists(directory)) {
        // Return empty vector to indicate the directory doesn't exist
        return jpgFiles;
    }
    
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        auto ext = entry.path().extension().string();
        if (ext == ".jpg" || ext == ".jpeg" ||
            ext == ".JPG" || ext == ".JPEG") { // Handle case insensitivity
            jpgFiles.emplace_back(entry.path().string());
        }
    }
    return jpgFiles;
}

 
bool deleteFile(const std::string& filename) {
    return std::filesystem::remove(filename);
}
