#include "Vehicle.h"
#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>

const std::string VEHICLE_FILE = "vehicles.dat";

Vehicle::Vehicle()
    : vehicleNumber(""), brand(""), model(""), year(0), color(""), engineNumber(""), customerId(0), isDeleted(false) {}

Vehicle::Vehicle(const std::string& vehicleNumber, const std::string& brand, const std::string& model,
                 int year, const std::string& color, const std::string& engineNumber, int customerId, bool isDeleted)
    : vehicleNumber(vehicleNumber), brand(brand), model(model), year(year), color(color), engineNumber(engineNumber),
      customerId(customerId), isDeleted(isDeleted) {}

Vehicle::Vehicle(const VehicleRecord& r)
    : vehicleNumber(r.vehicleNumber), brand(r.brand), model(r.model), year(r.year), color(r.color),
      engineNumber(r.engineNumber), customerId(r.customerId), isDeleted(r.isDeleted) {}

// Getters
std::string Vehicle::getVehicleNumber() const { return vehicleNumber; }
std::string Vehicle::getBrand() const { return brand; }
std::string Vehicle::getModel() const { return model; }
int Vehicle::getYear() const { return year; }
std::string Vehicle::getColor() const { return color; }
std::string Vehicle::getEngineNumber() const { return engineNumber; }
int Vehicle::getCustomerId() const { return customerId; }
bool Vehicle::getIsDeleted() const { return isDeleted; }

// Setters
void Vehicle::setVehicleNumber(const std::string& vehicleNumber) { this->vehicleNumber = vehicleNumber; }
void Vehicle::setBrand(const std::string& brand) { this->brand = brand; }
void Vehicle::setModel(const std::string& model) { this->model = model; }
void Vehicle::setYear(int year) { this->year = year; }
void Vehicle::setColor(const std::string& color) { this->color = color; }
void Vehicle::setEngineNumber(const std::string& engineNumber) { this->engineNumber = engineNumber; }
void Vehicle::setCustomerId(int customerId) { this->customerId = customerId; }
void Vehicle::setIsDeleted(bool isDeleted) { this->isDeleted = isDeleted; }

VehicleRecord Vehicle::toRecord() const {
    VehicleRecord r;
    r.year = year;
    r.customerId = customerId;
    r.isDeleted = isDeleted;

    std::memset(r.vehicleNumber, 0, sizeof(r.vehicleNumber));
    std::memset(r.brand, 0, sizeof(r.brand));
    std::memset(r.model, 0, sizeof(r.model));
    std::memset(r.color, 0, sizeof(r.color));
    std::memset(r.engineNumber, 0, sizeof(r.engineNumber));

    vehicleNumber.copy(r.vehicleNumber, sizeof(r.vehicleNumber) - 1);
    brand.copy(r.brand, sizeof(r.brand) - 1);
    model.copy(r.model, sizeof(r.model) - 1);
    color.copy(r.color, sizeof(r.color) - 1);
    engineNumber.copy(r.engineNumber, sizeof(r.engineNumber) - 1);

    return r;
}

bool Vehicle::findByNumber(const std::string& vehicleNumber, Vehicle& vehicle) {
    std::ifstream file(VEHICLE_FILE, std::ios::binary);
    if (!file) return false;

    std::string searchNum = Utils::trim(Utils::toLowerCase(vehicleNumber));
    VehicleRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(VehicleRecord))) {
        std::string recordNum = Utils::trim(Utils::toLowerCase(r.vehicleNumber));
        if (recordNum == searchNum && !r.isDeleted) {
            vehicle = Vehicle(r);
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

std::vector<Vehicle> Vehicle::findByCustomerId(int customerId) {
    std::vector<Vehicle> list;
    std::ifstream file(VEHICLE_FILE, std::ios::binary);
    if (!file) return list;

    VehicleRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(VehicleRecord))) {
        if (r.customerId == customerId && !r.isDeleted) {
            list.push_back(Vehicle(r));
        }
    }
    file.close();
    return list;
}

std::vector<Vehicle> Vehicle::getAllActiveVehicles() {
    std::vector<Vehicle> list;
    std::ifstream file(VEHICLE_FILE, std::ios::binary);
    if (!file) return list;

    VehicleRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(VehicleRecord))) {
        if (!r.isDeleted) {
            list.push_back(Vehicle(r));
        }
    }
    file.close();
    return list;
}

void Vehicle::displayCard() const {
    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "  Vehicle Details:\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  --------------------------------------------------\n";
    std::cout << "  Vehicle No   : " << vehicleNumber << "\n";
    std::cout << "  Brand & Model: " << brand << " " << model << "\n";
    std::cout << "  Year         : " << year << "\n";
    std::cout << "  Color        : " << color << "\n";
    std::cout << "  Engine No    : " << engineNumber << "\n";
    
    Customer owner;
    if (Customer::findById(customerId, owner)) {
        std::cout << "  Owner        : " << owner.getName() << " (ID: " << customerId << ")\n";
    } else {
        std::cout << "  Owner ID     : " << customerId << " (Not found/Deleted)\n";
    }
    std::cout << "  --------------------------------------------------\n";
    Utils::resetColor();
}

void Vehicle::registerVehicleUI() {
    Utils::printHeader("REGISTER VEHICLE");

    // First prompt for Owner ID
    int custId = Utils::getValidatedInt("  Enter Customer Owner ID: ");
    Customer owner;
    if (!Customer::findById(custId, owner)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Error: Customer with ID " << custId << " does not exist!\n";
        std::cout << "  Please register the Customer first.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    std::cout << "  Owner Found: " << owner.getName() << "\n\n";

    std::string rawVehNo = Utils::getNonEmptyString("  Enter Vehicle Number (e.g. MH12AB1234): ");
    // Convert to upper case for consistency
    std::string vehNo = Utils::trim(rawVehNo);
    std::transform(vehNo.begin(), vehNo.end(), vehNo.begin(), ::toupper);

    Vehicle temp;
    if (findByNumber(vehNo, temp)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Error: Vehicle with number " << vehNo << " is already registered!\n";
        temp.displayCard();
        Utils::pressEnterToContinue();
        return;
    }

    std::string brand = Utils::getNonEmptyString("  Enter Brand (e.g. Maruti, Honda): ");
    std::string model = Utils::getNonEmptyString("  Enter Model (e.g. Swift, Civic): ");
    int year = Utils::getValidatedInt("  Enter Manufacture Year: ", 1950, 2030);
    while (!Utils::isValidYear(year)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Year cannot exceed current date's calendar year.\n";
        Utils::resetColor();
        year = Utils::getValidatedInt("  Enter Manufacture Year: ", 1950, 2030);
    }

    std::string color = Utils::getNonEmptyString("  Enter Color: ");
    std::string engineNo = Utils::getNonEmptyString("  Enter Engine Number: ");

    Vehicle newVeh(vehNo, brand, model, year, color, engineNo, custId);
    VehicleRecord record = newVeh.toRecord();

    std::ofstream file(VEHICLE_FILE, std::ios::binary | std::ios::app);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open vehicle database for writing.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    file.write(reinterpret_cast<const char*>(&record), sizeof(VehicleRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Vehicle registered successfully!\n";
    newVeh.displayCard();
    Utils::pressEnterToContinue();
}

void Vehicle::displayAllVehiclesUI() {
    Utils::printHeader("VEHICLE REGISTRY");

    std::vector<Vehicle> vehicles = getAllActiveVehicles();
    if (vehicles.empty()) {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "  No vehicles registered in the system.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    const size_t pageSize = 5;
    size_t totalPages = (vehicles.size() + pageSize - 1) / pageSize;
    size_t currentPage = 0;

    while (true) {
        Utils::printHeader("VEHICLE REGISTRY (Page " + std::to_string(currentPage + 1) + " of " + std::to_string(totalPages) + ")");

        Utils::setColor(Utils::Color::BOLD_WHITE);
        std::cout << "  "
                  << Utils::padString("Veh No", 15) << " | "
                  << Utils::padString("Brand", 15) << " | "
                  << Utils::padString("Model", 15) << " | "
                  << Utils::padString("Owner ID", 8) << "\n";
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  ------------------------------------------------------------\n";
        Utils::resetColor();

        size_t startIdx = currentPage * pageSize;
        size_t endIdx = std::min(startIdx + pageSize, vehicles.size());

        for (size_t i = startIdx; i < endIdx; ++i) {
            std::cout << "  "
                      << Utils::padString(vehicles[i].getVehicleNumber(), 15) << " | "
                      << Utils::padString(vehicles[i].getBrand(), 15) << " | "
                      << Utils::padString(vehicles[i].getModel(), 15) << " | "
                      << Utils::padString(std::to_string(vehicles[i].getCustomerId()), 8) << "\n";
        }

        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  ------------------------------------------------------------\n";
        Utils::resetColor();

        std::cout << "  [N] Next Page  |  [P] Previous Page  |  [Q] Exit Registry\n";
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

void Vehicle::searchVehicleUI() {
    Utils::printHeader("SEARCH VEHICLE");
    std::cout << "  1. Search by Vehicle Number\n";
    std::cout << "  2. Search by Owner ID\n";
    std::cout << "  3. Back\n\n";

    int choice = Utils::getValidatedInt("  Enter Choice (1-3): ", 1, 3);
    if (choice == 3) return;

    if (choice == 1) {
        std::string vehNo = Utils::getNonEmptyString("  Enter Vehicle Number: ");
        Vehicle v;
        if (findByNumber(vehNo, v)) {
            std::cout << "\n";
            v.displayCard();
        } else {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  Vehicle with number " << vehNo << " not found.\n";
            Utils::resetColor();
        }
    } else if (choice == 2) {
        int custId = Utils::getValidatedInt("  Enter Customer Owner ID: ");
        std::vector<Vehicle> list = findByCustomerId(custId);

        if (list.empty()) {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  No vehicles registered under Owner ID " << custId << ".\n";
            Utils::resetColor();
        } else {
            std::cout << "\n  Found " << list.size() << " vehicle(s) for Owner ID " << custId << ":\n\n";
            for (const auto& v : list) {
                v.displayCard();
                std::cout << "\n";
            }
        }
    }
    Utils::pressEnterToContinue();
}

void Vehicle::updateVehicleUI() {
    Utils::printHeader("UPDATE VEHICLE DETAILS");
    std::string vehNo = Utils::getNonEmptyString("  Enter Vehicle Number to update: ");
    std::string searchNum = Utils::trim(Utils::toLowerCase(vehNo));

    std::fstream file(VEHICLE_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    VehicleRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(VehicleRecord))) {
        std::string recordNum = Utils::trim(Utils::toLowerCase(r.vehicleNumber));
        if (recordNum == searchNum && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Vehicle with number " << vehNo << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        file.close();
        return;
    }

    Vehicle current(r);
    std::cout << "\n  Current Info:\n";
    current.displayCard();

    std::cout << "  Enter new details (leave blank to keep current):\n\n";

    std::string newBrand = Utils::trim(Utils::getNonEmptyString("  New Brand [" + current.getBrand() + "]: "));
    if (newBrand.empty() || newBrand == "[" + current.getBrand() + "]:") { // safety check for enter fallback
        // We'll read line manually to allow true empty input
    }
    
    // Let's do it via getline
    std::cout << "  New Brand [" << current.getBrand() << "]: ";
    std::string nBrand;
    std::getline(std::cin, nBrand);
    nBrand = Utils::trim(nBrand);
    if (nBrand.empty()) nBrand = current.getBrand();

    std::cout << "  New Model [" << current.getModel() << "]: ";
    std::string nModel;
    std::getline(std::cin, nModel);
    nModel = Utils::trim(nModel);
    if (nModel.empty()) nModel = current.getModel();

    std::string nYearStr;
    int nYear = current.getYear();
    while (true) {
        std::cout << "  New Year [" << current.getYear() << "]: ";
        std::getline(std::cin, nYearStr);
        nYearStr = Utils::trim(nYearStr);
        if (nYearStr.empty()) {
            break;
        }
        try {
            int parsedYear = std::stoi(nYearStr);
            if (Utils::isValidYear(parsedYear) && parsedYear >= 1950) {
                nYear = parsedYear;
                break;
            }
        } catch (...) {}
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Invalid year. Must be between 1950 and current calendar year.\n";
        Utils::resetColor();
    }

    std::cout << "  New Color [" << current.getColor() << "]: ";
    std::string nColor;
    std::getline(std::cin, nColor);
    nColor = Utils::trim(nColor);
    if (nColor.empty()) nColor = current.getColor();

    std::cout << "  New Engine No [" << current.getEngineNumber() << "]: ";
    std::string nEngine;
    std::getline(std::cin, nEngine);
    nEngine = Utils::trim(nEngine);
    if (nEngine.empty()) nEngine = current.getEngineNumber();

    std::string nOwnerStr;
    int nOwnerId = current.getCustomerId();
    while (true) {
        std::cout << "  New Owner Customer ID [" << current.getCustomerId() << "]: ";
        std::getline(std::cin, nOwnerStr);
        nOwnerStr = Utils::trim(nOwnerStr);
        if (nOwnerStr.empty()) {
            break;
        }
        try {
            int parsedId = std::stoi(nOwnerStr);
            Customer testOwner;
            if (Customer::findById(parsedId, testOwner)) {
                nOwnerId = parsedId;
                break;
            }
        } catch (...) {}
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Customer ID does not exist in directory.\n";
        Utils::resetColor();
    }

    Vehicle updatedVeh(current.getVehicleNumber(), nBrand, nModel, nYear, nColor, nEngine, nOwnerId);
    VehicleRecord updatedRecord = updatedVeh.toRecord();

    file.seekp(recordIndex * sizeof(VehicleRecord));
    file.write(reinterpret_cast<const char*>(&updatedRecord), sizeof(VehicleRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Vehicle updated successfully!\n";
    updatedVeh.displayCard();
    Utils::pressEnterToContinue();
}

void Vehicle::deleteVehicleUI() {
    Utils::printHeader("DELETE VEHICLE REGISTRATION");
    std::string vehNo = Utils::getNonEmptyString("  Enter Vehicle Number to delete: ");
    std::string searchNum = Utils::trim(Utils::toLowerCase(vehNo));

    std::fstream file(VEHICLE_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    VehicleRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(VehicleRecord))) {
        std::string recordNum = Utils::trim(Utils::toLowerCase(r.vehicleNumber));
        if (recordNum == searchNum && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Vehicle with number " << vehNo << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        file.close();
        return;
    }

    Vehicle target(r);
    std::cout << "\n";
    target.displayCard();

    bool confirm = Utils::getConfirmation("  Are you sure you want to delete this vehicle registration?");
    if (confirm) {
        r.isDeleted = true;
        file.seekp(recordIndex * sizeof(VehicleRecord));
        file.write(reinterpret_cast<const char*>(&r), sizeof(VehicleRecord));

        Utils::setColor(Utils::Color::GREEN);
        std::cout << "\n  Vehicle deleted successfully.\n";
        Utils::resetColor();
    } else {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "\n  Deletion cancelled.\n";
        Utils::resetColor();
    }

    file.close();
    Utils::pressEnterToContinue();
}
