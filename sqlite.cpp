#include "functions.h"
#include <sqlite3.h>
#include <iostream>
#include <ctime>
#include <limits>  // for date 
//for sending BLOB to sqlite3 
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iterator>



sqlite3* db = nullptr;

void initializeDatabase() {
    if (sqlite3_open("../../images.db", &db)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        exit(1);
    }

    const char* createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS Images (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT,
            hashtags TEXT,
            category INTEGER,
            isActive BOOLEAN,
            isHidden BOOLEAN,
            date TEXT,
            image BLOB
        );
    )";

    char* errMsg = nullptr;
    if (sqlite3_exec(db, createTableSQL, 0, 0, &errMsg) != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        exit(1);
    }
}



std::string getCurrentDate() {
    std::time_t t = std::time(nullptr);
    char date[11];  // Format: YYYY-MM-DD
    std::strftime(date, sizeof(date), "%Y-%m-%d", std::localtime(&t));
    return std::string(date);
}


std::vector<unsigned char> readFileAsBlob(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return {};
    }
    return std::vector<unsigned char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void insertImageData(sqlite3* db, const std::string& title, const std::string& hashtags, int category, bool isActive, bool isHidden, const std::string& date, const std::vector<unsigned char>& imageData) {
    const char* insertSQL = "INSERT INTO Images (title, hashtags, category, isActive, isHidden, date, image) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, insertSQL, -1, &stmt, 0) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, hashtags.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, category);
    sqlite3_bind_int(stmt, 4, isActive ? 1 : 0);
    sqlite3_bind_int(stmt, 5, isHidden ? 1 : 0);
    sqlite3_bind_text(stmt, 6, date.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_blob(stmt, 7, imageData.data(), imageData.size(), SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}
