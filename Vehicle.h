#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>
#include <vector>

struct VehicleRecord {
    char vehicleNumber[20];
    char brand[30];
    char model[30];
    int year;
    char color[20];
    char engineNumber[30];
    int customerId; // Linked Customer ID
    bool isDeleted;
};

class Vehicle {
private:
    std::string vehicleNumber;
    std::string brand;
    std::string model;
    int year;
    std::string color;
    std::string engineNumber;
    int customerId;
    bool isDeleted;

public:
    Vehicle();
    Vehicle(const std::string& vehicleNumber, const std::string& brand, const std::string& model,
            int year, const std::string& color, const std::string& engineNumber, int customerId, bool isDeleted = false);
    Vehicle(const VehicleRecord& record);

    // Getters
    std::string getVehicleNumber() const;
    std::string getBrand() const;
    std::string getModel() const;
    int getYear() const;
    std::string getColor() const;
    std::string getEngineNumber() const;
    int getCustomerId() const;
    bool getIsDeleted() const;

    // Setters
    void setVehicleNumber(const std::string& vehicleNumber);
    void setBrand(const std::string& brand);
    void setModel(const std::string& model);
    void setYear(int year);
    void setColor(const std::string& color);
    void setEngineNumber(const std::string& engineNumber);
    void setCustomerId(int customerId);
    void setIsDeleted(bool isDeleted);

    // Record conversion
    VehicleRecord toRecord() const;

    // Static CRUD/Management Operations
    static void registerVehicleUI();
    static void displayAllVehiclesUI();
    static void searchVehicleUI();
    static void updateVehicleUI();
    static void deleteVehicleUI();

    // Static Query Helpers
    static bool findByNumber(const std::string& vehicleNumber, Vehicle& vehicle);
    static std::vector<Vehicle> findByCustomerId(int customerId);
    static std::vector<Vehicle> getAllActiveVehicles();

    // UI display helpers
    void displayCard() const;
};

#endif // VEHICLE_H
