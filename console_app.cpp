#include "functions.h"
#include <iostream>
#include <string>
#include <cstdlib> // for system clear
#include <vector>
#include <filesystem> // Added for filesystem operations



void synapse_effect() {
    std::cout << "\033[36m" << R"(
   ,-,--.                   .-._            ,---.           _ __       ,-,--.        ,----.  
 ,-.'-  _\  ,--.-.  .-,--. /==/ \  .-._   .--.'  \       .-`.' ,`.   ,-.'-  _\    ,-.--` , \ 
/==/_ ,_.' /==/- / /=/_ /  |==|, \/ /, /  \==\-/\ \     /==/, -   \ /==/_ ,_.'   |==|-  _.-` 
\==\  \    \==\, \/=/. /   |==|-  \|  |   /==/-|_\ |   |==| _ .=. | \==\  \      |==|   `.-. 
 \==\ -\    \==\  \/ -/    |==| ,  | -|   \==\,   - \  |==| , '=',|  \==\ -\    /==/_ ,    / 
 _\==\ ,\    |==|  ,_/     |==| -   _ |   /==/ -   ,|  |==|-  '..'   _\==\ ,\   |==|    .-'  
/==/\/ _ |   \==\-, /      |==|  /\ , |  /==/-  /\ - \ |==|,  |     /==/\/ _ |  |==|_  ,`-._ 
\==\ - , /   /==/._/       /==/, | |- |  \==\ _.\=\.-' /==/ - |     \==\ - , /  /==/ ,     / 
 `--`---'    `--`-`        `--`./  `--`   `--`         `--`---'      `--`---'   `--`-----``  
)" << "\033[0m" << std::endl;
}



int main() {
    std::string input_file, output_file;
    int quality;
    char repeat;

    synapse_effect();

    std::cout << "\033[1;35m=== SYNAPSE Console App ===\033[0m" << std::endl;

    // Get input file path
    std::cout << "\033[1;33mEnter directory: \033[0m";  // Yellow
    std::getline(std::cin, input_file);
    input_file = "../../" + input_file;

    std::vector<std::string> files_vector = getJpgFiles(input_file);

    if (files_vector.empty()) {
        std::cout << "No JPEG files found in the directory.\n";
        return 1;
    }

    for (size_t i = 0; i < files_vector.size(); ++i) {
        std::string output_path;
        bool fileExists;
        do {
            std::filesystem::path input_path = std::filesystem::path(files_vector[i]);
            displayImage(input_path.string());
            std::cout << "\033[1;34mEnter File name: \033[0m";  // Blue
            std::cin >> output_file;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore leftover input
            output_file += ".jpg";
            output_path = (std::filesystem::path("../../output") / output_file).string();


            fileExists = std::filesystem::exists(output_path);
            if (fileExists) {
                std::cout << "File already exists in output directory. Please enter a different file name.\n";
            }
        } while (fileExists);

        do {
            // Get compression quality
            std::cout << "\033[1;34mEnter compression quality (0-100): \033[0m";  // Blue
            std::cin >> quality;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore leftover input

            if (std::cin.fail() || quality < 0 || quality > 100) {
                std::cin.clear(); // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
                std::cout << "Invalid quality value. Please enter a number between 0 and 100.\n";
                continue;
            }

            // Call compression function
            compress_jpeg(files_vector[i], output_file, quality);

            displayImage(output_path);

            // Ask if the user wants to repeat
            do {
                std::cout << "\033[1;36mDo you confirm this compression? (y/n): \033[0m";  // Cyan
                std::cin >> repeat;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore leftover input

                if (repeat != 'y' && repeat != 'Y' && repeat != 'n' && repeat != 'N') {
                    std::cout << "Invalid input. Please enter 'y' or 'n'.\n";
                }
            } while (repeat != 'y' && repeat != 'Y' && repeat != 'n' && repeat != 'N');

            if (repeat == 'n' || repeat == 'N') {
                deleteFile(output_path); // Delete the previously compressed file
            }

        } while (repeat == 'n' || repeat == 'N');

        deleteFile(files_vector[i]); // Move inside the for loop
    
        

        initializeDatabase(); // Initialize the database first

        if (!db) {
            std::cerr << "Database not initialized.\n";
            return 1;
        }




        std::string title, hashtags , date ;
        int category;
        bool isActive , isHidden;    //active -> user will be notified about note  



        date = getCurrentDate();


        // After confirming compression
        std::cout << "\033[1;34mEnter title: \033[0m";
        std::getline(std::cin, title);

        std::cout << "\033[1;34mEnter hashtags: \033[0m";
        std::getline(std::cin, hashtags);

        do {
            std::cout << "\033[1;34mEnter category (1-3): \033[0m";
            std::cin >> category;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (category < 1 || category > 3) {
                std::cout << "Invalid category. Please enter a number between 1 and 3.\n";
            }
        } while (category < 1 || category > 3);

        char activeInput;
        do {
            std::cout << "\033[1;34mIs active? (1 for true, 0 for false): \033[0m";
            std::cin >> activeInput;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (activeInput == '1') {
                isActive = true;
            } else if (activeInput == '0') {
                isActive = false;
            } else {
                std::cout << "Invalid input. Please enter 1 or 0.\n";
            }
        } while (activeInput != '1' && activeInput != '0');

        
        char hiddenInput;
        do {
            std::cout << "\033[1;34mDo you want to hide it? (1 for true, 0 for false): \033[0m";
            std::cin >> hiddenInput;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (hiddenInput == '1') {
                isHidden = true;
            } else if (hiddenInput == '0') {
                isHidden = false;
            } else {
                std::cout << "Invalid input. Please enter 1 or 0.\n";
            }
        } while (hiddenInput != '1' && hiddenInput != '0');


        std::vector<unsigned char> imageBlob = readFileAsBlob(output_path);
        if (!imageBlob.empty()) {
            insertImageData(db, title, hashtags, category, isActive,isHidden, date, imageBlob);
            std::cout << "\033[1;32mImage and metadata successfully saved to database.\033[0m\n"; 

        }
        sqlite3_close(db);

        }
    return 0;
}