#include "Part.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>

const std::string PART_FILE = "parts.dat";

Part::Part()
    : id(0), name(""), unitPrice(0.0), quantity(0), isDeleted(false) {}

Part::Part(int id, const std::string& name, double unitPrice, int quantity, bool isDeleted)
    : id(id), name(name), unitPrice(unitPrice), quantity(quantity), isDeleted(isDeleted) {}

Part::Part(const PartRecord& r)
    : id(r.id), name(r.name), unitPrice(r.unitPrice), quantity(r.quantity), isDeleted(r.isDeleted) {}

// Getters
int Part::getId() const { return id; }
std::string Part::getName() const { return name; }
double Part::getUnitPrice() const { return unitPrice; }
int Part::getQuantity() const { return quantity; }
bool Part::getIsDeleted() const { return isDeleted; }

// Setters
void Part::setId(int id) { this->id = id; }
void Part::setName(const std::string& name) { this->name = name; }
void Part::setUnitPrice(double unitPrice) { this->unitPrice = unitPrice; }
void Part::setQuantity(int quantity) { this->quantity = quantity; }
void Part::setIsDeleted(bool isDeleted) { this->isDeleted = isDeleted; }

PartRecord Part::toRecord() const {
    PartRecord r;
    r.id = id;
    r.unitPrice = unitPrice;
    r.quantity = quantity;
    r.isDeleted = isDeleted;

    std::memset(r.name, 0, sizeof(r.name));
    name.copy(r.name, sizeof(r.name) - 1);

    return r;
}

int Part::getNextId() {
    std::ifstream file(PART_FILE, std::ios::binary);
    if (!file) return 1;

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    if (fileSize == 0) return 1;

    int totalRecords = fileSize / sizeof(PartRecord);
    file.seekg((totalRecords - 1) * sizeof(PartRecord));

    PartRecord r;
    if (file.read(reinterpret_cast<char*>(&r), sizeof(PartRecord))) {
        return r.id + 1;
    }
    return 1;
}

bool Part::findById(int id, Part& part) {
    std::ifstream file(PART_FILE, std::ios::binary);
    if (!file) return false;

    PartRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(PartRecord))) {
        if (r.id == id && !r.isDeleted) {
            part = Part(r);
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

bool Part::updateStock(int id, int qtyDeducted) {
    std::fstream file(PART_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) return false;

    PartRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(PartRecord))) {
        if (r.id == id && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (found) {
        if (r.quantity >= qtyDeducted) {
            r.quantity -= qtyDeducted;
            file.seekp(recordIndex * sizeof(PartRecord));
            file.write(reinterpret_cast<const char*>(&r), sizeof(PartRecord));
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

std::vector<Part> Part::getAllActiveParts() {
    std::vector<Part> list;
    std::ifstream file(PART_FILE, std::ios::binary);
    if (!file) return list;

    PartRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(PartRecord))) {
        if (!r.isDeleted) {
            list.push_back(Part(r));
        }
    }
    file.close();
    return list;
}

void Part::displayCard() const {
    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "  Part Details:\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  --------------------------------------------------\n";
    std::cout << "  Part ID     : " << id << "\n";
    std::cout << "  Part Name   : " << name << "\n";
    std::cout << "  Unit Price  : Rs " << std::fixed << std::setprecision(2) << unitPrice << "\n";
    std::cout << "  Stock Qty   : " << quantity << "\n";
    std::cout << "  --------------------------------------------------\n";
    Utils::resetColor();
}

void Part::addPartUI() {
    Utils::printHeader("ADD SPARE PART");

    int id = getNextId();
    std::cout << "  Generating Part ID: " << id << "\n\n";

    std::string name = Utils::getNonEmptyString("  Enter Part Name: ");
    double price = Utils::getValidatedDouble("  Enter Unit Price: ", 0.0);
    int qty = Utils::getValidatedInt("  Enter Initial Stock Quantity: ", 0);

    Part newPart(id, name, price, qty);
    PartRecord record = newPart.toRecord();

    std::ofstream file(PART_FILE, std::ios::binary | std::ios::app);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file for writing.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    file.write(reinterpret_cast<const char*>(&record), sizeof(PartRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Spare Part added successfully!\n";
    newPart.displayCard();
    Utils::pressEnterToContinue();
}

void Part::displayAllPartsUI() {
    Utils::printHeader("SPARE PARTS INVENTORY");

    std::vector<Part> parts = getAllActiveParts();
    if (parts.empty()) {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "  No spare parts in inventory.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    const size_t pageSize = 5;
    size_t totalPages = (parts.size() + pageSize - 1) / pageSize;
    size_t currentPage = 0;

    while (true) {
        Utils::printHeader("SPARE PARTS INVENTORY (Page " + std::to_string(currentPage + 1) + " of " + std::to_string(totalPages) + ")");

        Utils::setColor(Utils::Color::BOLD_WHITE);
        std::cout << "  "
                  << Utils::padString("ID", 5) << " | "
                  << Utils::padString("Part Name", 25) << " | "
                  << Utils::padString("Unit Price (Rs)", 18) << " | "
                  << Utils::padString("Stock Qty", 10) << "\n";
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  ----------------------------------------------------------------------\n";
        Utils::resetColor();

        size_t startIdx = currentPage * pageSize;
        size_t endIdx = std::min(startIdx + pageSize, parts.size());

        for (size_t i = startIdx; i < endIdx; ++i) {
            std::cout << "  "
                      << Utils::padString(std::to_string(parts[i].getId()), 5) << " | "
                      << Utils::padString(parts[i].getName(), 25) << " | "
                      << Utils::padString(std::to_string(int(parts[i].getUnitPrice())), 18) << " | "
                      << Utils::padString(std::to_string(parts[i].getQuantity()), 10) << "\n";
        }

        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  ----------------------------------------------------------------------\n";
        Utils::resetColor();

        std::cout << "  [N] Next Page  |  [P] Previous Page  |  [Q] Exit Inventory\n";
        std::string choice = Utils::getNonEmptyString("  Enter Choice: ");
        choice = Utils::toLowerCase(choice);

        if (choice == "n") {
            if (currentPage + 1 < totalPages) currentPage++;
        } else if (choice == "p") {
            if (currentPage > 0) currentPage--;
        } else if (choice == "q") {
            break;
        }
    }
}

void Part::searchPartUI() {
    Utils::printHeader("SEARCH INVENTORY");
    std::cout << "  1. Search by Part ID\n";
    std::cout << "  2. Search by Name (Partial Match)\n";
    std::cout << "  3. Back\n\n";

    int choice = Utils::getValidatedInt("  Enter Choice (1-3): ", 1, 3);
    if (choice == 3) return;

    if (choice == 1) {
        int targetId = Utils::getValidatedInt("  Enter Part ID: ");
        Part part;
        if (findById(targetId, part)) {
            std::cout << "\n";
            part.displayCard();
        } else {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  Spare part with ID " << targetId << " not found.\n";
            Utils::resetColor();
        }
    } else if (choice == 2) {
        std::string searchName = Utils::getNonEmptyString("  Enter Part Name (or keyword): ");
        searchName = Utils::toLowerCase(searchName);
        std::vector<Part> list = getAllActiveParts();
        std::vector<Part> results;

        for (const auto& part : list) {
            std::string currentName = Utils::toLowerCase(part.getName());
            if (currentName.find(searchName) != std::string::npos) {
                results.push_back(part);
            }
        }

        if (results.empty()) {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  No spare parts match the keyword \"" << searchName << "\".\n";
            Utils::resetColor();
        } else {
            std::cout << "\n  Found " << results.size() << " match(es):\n\n";
            for (const auto& part : results) {
                part.displayCard();
                std::cout << "\n";
            }
        }
    }
    Utils::pressEnterToContinue();
}

void Part::updatePartUI() {
    Utils::printHeader("UPDATE SPARE PART");
    int targetId = Utils::getValidatedInt("  Enter Part ID to update: ");

    std::fstream file(PART_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    PartRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(PartRecord))) {
        if (r.id == targetId && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Spare part with ID " << targetId << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        file.close();
        return;
    }

    Part current(r);
    std::cout << "\n  Current Info:\n";
    current.displayCard();

    std::cout << "  Enter new details (leave blank to keep current):\n\n";

    std::cout << "  New Name [" << current.getName() << "]: ";
    std::string nName;
    std::getline(std::cin, nName);
    nName = Utils::trim(nName);
    if (nName.empty()) nName = current.getName();

    std::string nPriceStr;
    double nPrice = current.getUnitPrice();
    while (true) {
        std::cout << "  New Unit Price [" << current.getUnitPrice() << "]: ";
        std::getline(std::cin, nPriceStr);
        nPriceStr = Utils::trim(nPriceStr);
        if (nPriceStr.empty()) {
            break;
        }
        try {
            double val = std::stod(nPriceStr);
            if (val >= 0) {
                nPrice = val;
                break;
            }
        } catch (...) {}
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Unit Price must be a positive decimal.\n";
        Utils::resetColor();
    }

    std::string nQtyStr;
    int nQty = current.getQuantity();
    while (true) {
        std::cout << "  New Stock Quantity [" << current.getQuantity() << "]: ";
        std::getline(std::cin, nQtyStr);
        nQtyStr = Utils::trim(nQtyStr);
        if (nQtyStr.empty()) {
            break;
        }
        try {
            int val = std::stoi(nQtyStr);
            if (val >= 0) {
                nQty = val;
                break;
            }
        } catch (...) {}
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Stock Quantity must be a positive integer.\n";
        Utils::resetColor();
    }

    Part updatedPart(targetId, nName, nPrice, nQty);
    PartRecord updatedRecord = updatedPart.toRecord();

    file.seekp(recordIndex * sizeof(PartRecord));
    file.write(reinterpret_cast<const char*>(&updatedRecord), sizeof(PartRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Spare part updated successfully!\n";
    updatedPart.displayCard();
    Utils::pressEnterToContinue();
}

void Part::deletePartUI() {
    Utils::printHeader("DELETE SPARE PART");
    int targetId = Utils::getValidatedInt("  Enter Part ID to delete: ");

    std::fstream file(PART_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    PartRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(PartRecord))) {
        if (r.id == targetId && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Spare part with ID " << targetId << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        file.close();
        return;
    }

    Part target(r);
    std::cout << "\n";
    target.displayCard();

    bool confirm = Utils::getConfirmation("  Are you sure you want to delete this spare part record?");
    if (confirm) {
        r.isDeleted = true;
        file.seekp(recordIndex * sizeof(PartRecord));
        file.write(reinterpret_cast<const char*>(&r), sizeof(PartRecord));

        Utils::setColor(Utils::Color::GREEN);
        std::cout << "\n  Spare part deleted successfully.\n";
        Utils::resetColor();
    } else {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "\n  Deletion cancelled.\n";
        Utils::resetColor();
    }

    file.close();
    Utils::pressEnterToContinue();
}
