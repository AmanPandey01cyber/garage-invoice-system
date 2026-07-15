#include "Service.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>

const std::string SERVICE_FILE = "services.dat";

Service::Service()
    : id(0), name(""), description(""), labourCost(0.0), isDeleted(false) {}

Service::Service(int id, const std::string& name, const std::string& description, double labourCost, bool isDeleted)
    : id(id), name(name), description(description), labourCost(labourCost), isDeleted(isDeleted) {}

Service::Service(const ServiceRecord& r)
    : id(r.id), name(r.name), description(r.description), labourCost(r.labourCost), isDeleted(r.isDeleted) {}

// Getters
int Service::getId() const { return id; }
std::string Service::getName() const { return name; }
std::string Service::getDescription() const { return description; }
double Service::getLabourCost() const { return labourCost; }
bool Service::getIsDeleted() const { return isDeleted; }

// Setters
void Service::setId(int id) { this->id = id; }
void Service::setName(const std::string& name) { this->name = name; }
void Service::setDescription(const std::string& description) { this->description = description; }
void Service::setLabourCost(double labourCost) { this->labourCost = labourCost; }
void Service::setIsDeleted(bool isDeleted) { this->isDeleted = isDeleted; }

ServiceRecord Service::toRecord() const {
    ServiceRecord r;
    r.id = id;
    r.labourCost = labourCost;
    r.isDeleted = isDeleted;

    std::memset(r.name, 0, sizeof(r.name));
    std::memset(r.description, 0, sizeof(r.description));

    name.copy(r.name, sizeof(r.name) - 1);
    description.copy(r.description, sizeof(r.description) - 1);

    return r;
}

void Service::prepopulateServices() {
    std::ifstream file(SERVICE_FILE, std::ios::binary);
    if (file) {
        file.seekg(0, std::ios::end);
        if (file.tellg() > 0) {
            // Already populated
            file.close();
            return;
        }
        file.close();
    }

    std::vector<Service> defaults = {
        Service(1, "Oil Change", "Engine oil and filter change", 500.0),
        Service(2, "Brake Service", "Brake pads inspect and replacement", 800.0),
        Service(3, "Wheel Alignment", "Laser wheel balancing and alignment", 600.0),
        Service(4, "Battery Replacement", "12V battery test and change", 300.0),
        Service(5, "Engine Repair", "Tune-up, gaskets and general overhaul", 5000.0),
        Service(6, "Car Wash", "Exterior clean, vacuum and polish", 400.0),
        Service(7, "AC Repair", "Refrigerant recharge and filter check", 1500.0),
        Service(8, "Denting", "Panel beating and alignment work", 2500.0),
        Service(9, "Painting", "Premium primer and paint finish per panel", 4000.0),
        Service(10, "Full Service", "Complete 40-point vehicle diagnosis", 3500.0)
    };

    std::ofstream outFile(SERVICE_FILE, std::ios::binary | std::ios::trunc);
    if (!outFile) return;

    for (const auto& s : defaults) {
        ServiceRecord rec = s.toRecord();
        outFile.write(reinterpret_cast<const char*>(&rec), sizeof(ServiceRecord));
    }
    outFile.close();
}

int Service::getNextId() {
    std::ifstream file(SERVICE_FILE, std::ios::binary);
    if (!file) return 1;

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    if (fileSize == 0) return 1;

    int totalRecords = fileSize / sizeof(ServiceRecord);
    file.seekg((totalRecords - 1) * sizeof(ServiceRecord));

    ServiceRecord r;
    if (file.read(reinterpret_cast<char*>(&r), sizeof(ServiceRecord))) {
        return r.id + 1;
    }
    return 1;
}

bool Service::findById(int id, Service& srv) {
    std::ifstream file(SERVICE_FILE, std::ios::binary);
    if (!file) return false;

    ServiceRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(ServiceRecord))) {
        if (r.id == id && !r.isDeleted) {
            srv = Service(r);
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

std::vector<Service> Service::getAllActiveServices() {
    prepopulateServices(); // Make sure defaults are there
    std::vector<Service> list;
    std::ifstream file(SERVICE_FILE, std::ios::binary);
    if (!file) return list;

    ServiceRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(ServiceRecord))) {
        if (!r.isDeleted) {
            list.push_back(Service(r));
        }
    }
    file.close();
    return list;
}

void Service::displayCard() const {
    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "  Service Details:\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  --------------------------------------------------\n";
    std::cout << "  Service ID  : " << id << "\n";
    std::cout << "  Name        : " << name << "\n";
    std::cout << "  Description : " << description << "\n";
    std::cout << "  Labour Cost : Rs " << std::fixed << std::setprecision(2) << labourCost << "\n";
    std::cout << "  --------------------------------------------------\n";
    Utils::resetColor();
}

void Service::addServiceUI() {
    Utils::printHeader("ADD NEW SERVICE");

    int id = getNextId();
    std::cout << "  Generating Service ID: " << id << "\n\n";

    std::string name = Utils::getNonEmptyString("  Enter Service Name: ");
    std::string description = Utils::getNonEmptyString("  Enter Description: ");
    double cost = Utils::getValidatedDouble("  Enter Labour Cost: ", 0.0);

    Service newSrv(id, name, description, cost);
    ServiceRecord record = newSrv.toRecord();

    std::ofstream file(SERVICE_FILE, std::ios::binary | std::ios::app);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file for writing.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    file.write(reinterpret_cast<const char*>(&record), sizeof(ServiceRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Service added successfully!\n";
    newSrv.displayCard();
    Utils::pressEnterToContinue();
}

void Service::displayAllServicesUI() {
    Utils::printHeader("SERVICES CATALOG");

    std::vector<Service> services = getAllActiveServices();
    if (services.empty()) {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "  No services registered in the system.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    const size_t pageSize = 5;
    size_t totalPages = (services.size() + pageSize - 1) / pageSize;
    size_t currentPage = 0;

    while (true) {
        Utils::printHeader("SERVICES CATALOG (Page " + std::to_string(currentPage + 1) + " of " + std::to_string(totalPages) + ")");

        Utils::setColor(Utils::Color::BOLD_WHITE);
        std::cout << "  "
                  << Utils::padString("ID", 5) << " | "
                  << Utils::padString("Service Name", 25) << " | "
                  << Utils::padString("Labour Cost (Rs)", 18) << "\n";
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  --------------------------------------------------------\n";
        Utils::resetColor();

        size_t startIdx = currentPage * pageSize;
        size_t endIdx = std::min(startIdx + pageSize, services.size());

        for (size_t i = startIdx; i < endIdx; ++i) {
            std::cout << "  "
                      << Utils::padString(std::to_string(services[i].getId()), 5) << " | "
                      << Utils::padString(services[i].getName(), 25) << " | "
                      << Utils::padString(std::to_string(int(services[i].getLabourCost())), 18) << "\n";
        }

        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  --------------------------------------------------------\n";
        Utils::resetColor();

        std::cout << "  [N] Next Page  |  [P] Previous Page  |  [Q] Exit Catalog\n";
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

void Service::searchServiceUI() {
    Utils::printHeader("SEARCH SERVICE");
    std::cout << "  1. Search by Service ID\n";
    std::cout << "  2. Search by Name (Partial Match)\n";
    std::cout << "  3. Back\n\n";

    int choice = Utils::getValidatedInt("  Enter Choice (1-3): ", 1, 3);
    if (choice == 3) return;

    if (choice == 1) {
        int targetId = Utils::getValidatedInt("  Enter Service ID: ");
        Service srv;
        if (findById(targetId, srv)) {
            std::cout << "\n";
            srv.displayCard();
        } else {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  Service with ID " << targetId << " not found.\n";
            Utils::resetColor();
        }
    } else if (choice == 2) {
        std::string searchName = Utils::getNonEmptyString("  Enter Service Name (or keyword): ");
        searchName = Utils::toLowerCase(searchName);
        std::vector<Service> list = getAllActiveServices();
        std::vector<Service> results;

        for (const auto& s : list) {
            std::string currentName = Utils::toLowerCase(s.getName());
            if (currentName.find(searchName) != std::string::npos) {
                results.push_back(s);
            }
        }

        if (results.empty()) {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  No services match the keyword \"" << searchName << "\".\n";
            Utils::resetColor();
        } else {
            std::cout << "\n  Found " << results.size() << " match(es):\n\n";
            for (const auto& s : results) {
                s.displayCard();
                std::cout << "\n";
            }
        }
    }
    Utils::pressEnterToContinue();
}

void Service::updateServiceUI() {
    Utils::printHeader("UPDATE SERVICE DETAILS");
    int targetId = Utils::getValidatedInt("  Enter Service ID to update: ");

    std::fstream file(SERVICE_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    ServiceRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(ServiceRecord))) {
        if (r.id == targetId && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Service with ID " << targetId << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        file.close();
        return;
    }

    Service current(r);
    std::cout << "\n  Current Details:\n";
    current.displayCard();

    std::cout << "  Enter new details (leave blank to keep current):\n\n";

    std::cout << "  New Name [" << current.getName() << "]: ";
    std::string nName;
    std::getline(std::cin, nName);
    nName = Utils::trim(nName);
    if (nName.empty()) nName = current.getName();

    std::cout << "  New Description [" << current.getDescription() << "]: ";
    std::string nDesc;
    std::getline(std::cin, nDesc);
    nDesc = Utils::trim(nDesc);
    if (nDesc.empty()) nDesc = current.getDescription();

    std::string nCostStr;
    double nCost = current.getLabourCost();
    while (true) {
        std::cout << "  New Labour Cost [" << current.getLabourCost() << "]: ";
        std::getline(std::cin, nCostStr);
        nCostStr = Utils::trim(nCostStr);
        if (nCostStr.empty()) {
            break;
        }
        try {
            double val = std::stod(nCostStr);
            if (val >= 0) {
                nCost = val;
                break;
            }
        } catch (...) {}
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Labour Cost must be a positive decimal.\n";
        Utils::resetColor();
    }

    Service updatedSrv(targetId, nName, nDesc, nCost);
    ServiceRecord updatedRecord = updatedSrv.toRecord();

    file.seekp(recordIndex * sizeof(ServiceRecord));
    file.write(reinterpret_cast<const char*>(&updatedRecord), sizeof(ServiceRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Service updated successfully!\n";
    updatedSrv.displayCard();
    Utils::pressEnterToContinue();
}

void Service::deleteServiceUI() {
    Utils::printHeader("DELETE SERVICE");
    int targetId = Utils::getValidatedInt("  Enter Service ID to delete: ");

    std::fstream file(SERVICE_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    ServiceRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(ServiceRecord))) {
        if (r.id == targetId && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Service with ID " << targetId << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        file.close();
        return;
    }

    Service target(r);
    std::cout << "\n";
    target.displayCard();

    bool confirm = Utils::getConfirmation("  Are you sure you want to delete this service from catalog?");
    if (confirm) {
        r.isDeleted = true;
        file.seekp(recordIndex * sizeof(ServiceRecord));
        file.write(reinterpret_cast<const char*>(&r), sizeof(ServiceRecord));

        Utils::setColor(Utils::Color::GREEN);
        std::cout << "\n  Service deleted successfully.\n";
        Utils::resetColor();
    } else {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "\n  Deletion cancelled.\n";
        Utils::resetColor();
    }

    file.close();
    Utils::pressEnterToContinue();
}
