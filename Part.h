#ifndef PART_H
#define PART_H

#include <string>
#include <vector>

struct PartRecord {
    int id;
    char name[50];
    double unitPrice;
    int quantity;
    bool isDeleted;
};

class Part {
private:
    int id;
    std::string name;
    double unitPrice;
    int quantity;
    bool isDeleted;

public:
    Part();
    Part(int id, const std::string& name, double unitPrice, int quantity, bool isDeleted = false);
    Part(const PartRecord& record);

    // Getters
    int getId() const;
    std::string getName() const;
    double getUnitPrice() const;
    int getQuantity() const;
    bool getIsDeleted() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setUnitPrice(double unitPrice);
    void setQuantity(int quantity);
    void setIsDeleted(bool isDeleted);

    // Record conversion
    PartRecord toRecord() const;

    // Static CRUD/Inventory Operations
    static void addPartUI();
    static void displayAllPartsUI();
    static void searchPartUI();
    static void updatePartUI();
    static void deletePartUI();

    // Static Query & Stock Helpers
    static int getNextId();
    static bool findById(int id, Part& part);
    static bool updateStock(int id, int qtyDeducted);
    static std::vector<Part> getAllActiveParts();

    // UI display helpers
    void displayCard() const;
};

#endif // PART_H
