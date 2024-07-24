#pragma once

#include <iostream>
#include <sqlite3.h>

// SQLite3Wrapper is a singleton class that wraps SQLite3 functionality
class SQLite3Wrapper {
public:
    // Get the singleton instance of SQLite3Wrapper with the specified database name
    static SQLite3Wrapper& getInstance(const std::string& db_name = "key_value_store.db") {
        static SQLite3Wrapper instance(db_name);
        return instance;
    }

    // Delete copy constructor and assignment operator
    SQLite3Wrapper(const SQLite3Wrapper&) = delete;
    SQLite3Wrapper& operator=(const SQLite3Wrapper&) = delete;

private:
    // Private constructor that opens the database and creates the table if it doesn't exist
    SQLite3Wrapper(const std::string& db_name) {
        int rc = sqlite3_open(db_name.c_str(), &db);
        if (rc) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            db = nullptr;
        } else {
            createTable();
        }
    }

    // Destructor that closes the database
    ~SQLite3Wrapper() {
        if (db) {
            sqlite3_close(db);
        }
    }

    // Set a key-value pair in the database
    bool setValue(const std::string& key, const std::string& value) {
        std::string sql = "INSERT OR REPLACE INTO key_value (key, value) VALUES (?, ?);";
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Cannot prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }
        sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, value.c_str(), -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        return rc == SQLITE_DONE;
    }

    // Get the value associated with the specified key
    std::string getValue(const std::string& key) {
        std::string sql = "SELECT value FROM key_value WHERE key = ?;";
        sqlite3_stmt* stmt;
int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Cannot prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return "";
        }
        sqlite3_bind_text(stmt, 1, key.c_str(), -1, SQLITE_TRANSIENT);
        rc = sqlite3_step(stmt);
        std::string value;
        if(rc == SQLITE_ROW) {
            value = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        }
        sqlite3_finalize(stmt);
        return value;
    }

    // Create the key_value table if it doesn't exist
    void createTable() {
        std::string sql = "CREATE TABLE IF NOT EXISTS key_value (key TEXT PRIMARY KEY, value TEXT);";
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "Cannot create table: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
    }

    sqlite3* db;
};

/*
int main() {
    SQLite3Wrapper& db = SQLite3Wrapper::getInstance();

    // Set a key-value pair
    if (db.setValue("example_key", "example_value")) {
        std::cout << "Key-value pair set successfully." << std::endl;
    } else {
        std::cout << "Failed to set key-value pair." << std::endl;
    }

    // Get the value associated with the key
    std::string value = db.getValue("example_key");
    if (!value.empty()) {
        std::cout << "Value for 'example_key': " << value << std::endl;
    } else {
        std::cout << "Failed to get value for 'example_key'." << std::endl;
    }

    return 0;
}
*/