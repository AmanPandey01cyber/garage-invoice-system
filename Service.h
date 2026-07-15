#ifndef SERVICE_H
#define SERVICE_H

#include <string>
#include <vector>

struct ServiceRecord {
    int id;
    char name[50];
    char description[100];
    double labourCost;
    bool isDeleted;
};

class Service {
private:
    int id;
    std::string name;
    std::string description;
    double labourCost;
    bool isDeleted;

public:
    Service();
    Service(int id, const std::string& name, const std::string& description, double labourCost, bool isDeleted = false);
    Service(const ServiceRecord& record);

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getDescription() const;
    double getLabourCost() const;
    bool getIsDeleted() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setLabourCost(double labourCost);
    void setIsDeleted(bool isDeleted);

    // Record conversion
    ServiceRecord toRecord() const;

    // Static Prepopulation & CRUD Operations
    static void prepopulateServices();
    static void addServiceUI();
    static void displayAllServicesUI();
    static void searchServiceUI();
    static void updateServiceUI();
    static void deleteServiceUI();

    // Static Query Helpers
    static int getNextId();
    static bool findById(int id, Service& srv);
    static std::vector<Service> getAllActiveServices();

    // UI display helpers
    void displayCard() const;
};

#endif // SERVICE_H
