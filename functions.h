#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sqlite3.h>  
#include <vector>
#include <string>

extern sqlite3* db;

void compress_jpeg(const std::string& input_file, const std::string& output_file, int quality);
std::vector<std::string> getJpgFiles(const std::string& directory);
bool deleteFile(const std::string& filename);
void displayImage(const std::string& imagePath);
std::string getCurrentDate();
std::vector<unsigned char> readFileAsBlob(const std::string& filepath);
void insertImageData(sqlite3* db, const std::string& title, const std::string& hashtags, int category, bool isActive, bool isHidden, const std::string& date, const std::vector<unsigned char>& imageData);
void initializeDatabase();

#endif
