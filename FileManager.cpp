#include "FileManager.h"
#include "Customer.h"
#include "Vehicle.h"
#include "Employee.h"
#include "Service.h"
#include "Part.h"
#include "Invoice.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <iomanip>

namespace FileManager {

    bool fileExists(const std::string& filepath) {
        std::ifstream f(filepath, std::ios::binary);
        return f.good();
    }

    bool clearFile(const std::string& filepath) {
        std::ofstream f(filepath, std::ios::binary | std::ios::trunc);
        return f.good();
    }

    bool backupFile(const std::string& src, const std::string& dest) {
        std::ifstream source(src, std::ios::binary);
        if (!source) return false;
        std::ofstream destination(dest, std::ios::binary | std::ios::trunc);
        if (!destination) return false;

        destination << source.rdbuf();
        return true;
    }

    void initializeDatabases() {
        // Pre-create empty database files if they don't exist
        std::vector<std::string> files = {
            "customers.dat", "vehicles.dat", "employees.dat", 
            "parts.dat", "invoices.dat"
        };
        for (const auto& file : files) {
            if (!fileExists(file)) {
                std::ofstream f(file, std::ios::binary | std::ios::app);
            }
        }
        // Special case for services catalog: prepopulate with defaults
        Service::prepopulateServices();
    }

    void resetAllDatabasesUI() {
        Utils::printHeader("RESET SYSTEM DATABASES");
        Utils::setColor(Utils::Color::RED);
        std::cout << "  WARNING: This operation will permanently clear all customer records,\n";
        std::cout << "  vehicle records, payroll entries, inventory, and historical invoices!\n\n";
        Utils::resetColor();

        bool confirm1 = Utils::getConfirmation("  Are you sure you want to proceed with full reset?");
        if (!confirm1) {
            std::cout << "  Reset cancelled.\n";
            Utils::pressEnterToContinue();
            return;
        }

        Utils::setColor(Utils::Color::RED);
        bool confirm2 = Utils::getConfirmation("  CRITICAL CONFIRMATION: Type 'y' to confirm wipeout of ALL data");
        Utils::resetColor();

        if (confirm2) {
            Utils::showProgress("Wiping database tables", 1000);
            clearFile("customers.dat");
            clearFile("vehicles.dat");
            clearFile("employees.dat");
            clearFile("parts.dat");
            clearFile("invoices.dat");
            clearFile("services.dat");

            // Re-seed standard services
            Service::prepopulateServices();

            Utils::setColor(Utils::Color::GREEN);
            std::cout << "  All system databases reset and seeded successfully!\n";
            Utils::resetColor();
        } else {
            std::cout << "  Reset aborted.\n";
        }
        Utils::pressEnterToContinue();
    }

    std::vector<DBStats> getDatabaseStats() {
        std::vector<DBStats> statsList;

        auto getStats = [](const std::string& dbName, const std::string& filename, size_t recordSize) -> DBStats {
            DBStats stats = { dbName, filename, 0, 0, 0, 0 };
            
            std::ifstream file(filename, std::ios::binary);
            if (!file) return stats;

            file.seekg(0, std::ios::end);
            stats.fileSize = file.tellg();
            if (stats.fileSize == 0) return stats;

            stats.totalRecords = stats.fileSize / recordSize;
            file.seekg(0, std::ios::beg);

            // Dynamically count active/deleted records
            char* buffer = new char[recordSize];
            while (file.read(buffer, recordSize)) {
                // In all our Record structs, isDeleted is the very last bool parameter.
                // We offset to find isDeleted flag. For safety, since alignment differs,
                // we can map to actual model types if we read them, but since record types
                // are known sizes, we can read actual records by casting.
                
                // Let's do it type-safely by checking dbName:
                if (dbName == "Customers") {
                    CustomerRecord* rec = reinterpret_cast<CustomerRecord*>(buffer);
                    if (rec->isDeleted) stats.deletedRecords++;
                    else stats.activeRecords++;
                } else if (dbName == "Vehicles") {
                    VehicleRecord* rec = reinterpret_cast<VehicleRecord*>(buffer);
                    if (rec->isDeleted) stats.deletedRecords++;
                    else stats.activeRecords++;
                } else if (dbName == "Employees") {
                    EmployeeRecord* rec = reinterpret_cast<EmployeeRecord*>(buffer);
                    if (rec->isDeleted) stats.deletedRecords++;
                    else stats.activeRecords++;
                } else if (dbName == "Services") {
                    ServiceRecord* rec = reinterpret_cast<ServiceRecord*>(buffer);
                    if (rec->isDeleted) stats.deletedRecords++;
                    else stats.activeRecords++;
                } else if (dbName == "Parts") {
                    PartRecord* rec = reinterpret_cast<PartRecord*>(buffer);
                    if (rec->isDeleted) stats.deletedRecords++;
                    else stats.activeRecords++;
                } else if (dbName == "Invoices") {
                    InvoiceRecord* rec = reinterpret_cast<InvoiceRecord*>(buffer);
                    if (rec->isDeleted) stats.deletedRecords++;
                    else stats.activeRecords++;
                }
            }
            delete[] buffer;
            file.close();
            return stats;
        };

        statsList.push_back(getStats("Customers", "customers.dat", sizeof(CustomerRecord)));
        statsList.push_back(getStats("Vehicles", "vehicles.dat", sizeof(VehicleRecord)));
        statsList.push_back(getStats("Employees", "employees.dat", sizeof(EmployeeRecord)));
        statsList.push_back(getStats("Services", "services.dat", sizeof(ServiceRecord)));
        statsList.push_back(getStats("Parts", "parts.dat", sizeof(PartRecord)));
        statsList.push_back(getStats("Invoices", "invoices.dat", sizeof(InvoiceRecord)));

        return statsList;
    }

    void displayMaintenanceDashboardUI() {
        while (true) {
            Utils::printHeader("DATABASE MAINTENANCE");
            std::vector<DBStats> list = getDatabaseStats();

            Utils::setColor(Utils::Color::BOLD_WHITE);
            std::cout << "  "
                      << Utils::padString("Table Name", 15) << " | "
                      << Utils::padString("Filename", 15) << " | "
                      << Utils::padString("Total Recs", 12) << " | "
                      << Utils::padString("Active", 8) << " | "
                      << Utils::padString("Deleted", 8) << " | "
                      << Utils::padString("Size (Bytes)", 12) << "\n";
            Utils::setColor(Utils::Color::CYAN);
            std::cout << "  --------------------------------------------------------------------------------\n";
            Utils::resetColor();

            for (const auto& s : list) {
                std::cout << "  "
                          << Utils::padString(s.dbName, 15) << " | "
                          << Utils::padString(s.filename, 15) << " | "
                          << Utils::padString(std::to_string(s.totalRecords), 12) << " | "
                          << Utils::padString(std::to_string(s.activeRecords), 8) << " | "
                          << Utils::padString(std::to_string(s.deletedRecords), 8) << " | "
                          << Utils::padString(std::to_string(s.fileSize), 12) << "\n";
            }

            Utils::setColor(Utils::Color::CYAN);
            std::cout << "  --------------------------------------------------------------------------------\n";
            Utils::resetColor();

            std::cout << "  1. Backup Database Files\n";
            std::cout << "  2. Reset All Databases (Wipe Data)\n";
            std::cout << "  3. Return to Admin Dashboard\n\n";

            int choice = Utils::getValidatedInt("  Enter Choice (1-3): ", 1, 3);
            if (choice == 3) break;

            if (choice == 1) {
                Utils::showProgress("Creating backup snapshots", 800);
                bool success = true;
                success &= backupFile("customers.dat", "customers_backup.dat");
                success &= backupFile("vehicles.dat", "vehicles_backup.dat");
                success &= backupFile("employees.dat", "employees_backup.dat");
                success &= backupFile("services.dat", "services_backup.dat");
                success &= backupFile("parts.dat", "parts_backup.dat");
                success &= backupFile("invoices.dat", "invoices_backup.dat");

                if (success) {
                    Utils::setColor(Utils::Color::GREEN);
                    std::cout << "\n  Database backups created successfully (*_backup.dat)!\n";
                    Utils::resetColor();
                } else {
                    Utils::setColor(Utils::Color::RED);
                    std::cout << "\n  Error: Some backups could not be created.\n";
                    Utils::resetColor();
                }
                Utils::pressEnterToContinue();
            } 
            else if (choice == 2) {
                resetAllDatabasesUI();
            }
        }
    }
}
