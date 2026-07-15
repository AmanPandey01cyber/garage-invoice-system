#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>

namespace FileManager {
    
    struct DBStats {
        std::string dbName;
        std::string filename;
        int totalRecords;
        int activeRecords;
        int deletedRecords;
        size_t fileSize;
    };

    // System Utilities
    bool fileExists(const std::string& filepath);
    bool clearFile(const std::string& filepath);
    bool backupFile(const std::string& src, const std::string& dest);
    
    // Database Health Checks
    void initializeDatabases();
    void resetAllDatabasesUI();
    
    // Reporting & Maintenance Dashboard
    std::vector<DBStats> getDatabaseStats();
    void displayMaintenanceDashboardUI();
}

#endif // FILEMANAGER_H
