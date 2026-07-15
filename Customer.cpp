#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>

const std::string CUSTOMER_FILE = "customers.dat";

Customer::Customer() : id(0), name(""), phone(""), email(""), address(""), isDeleted(false) {}

Customer::Customer(int id, const std::string& name, const std::string& phone, const std::string& email, const std::string& address, bool isDeleted)
    : id(id), name(name), phone(phone), email(email), address(address), isDeleted(isDeleted) {}

Customer::Customer(const CustomerRecord& r)
    : id(r.id), name(r.name), phone(r.phone), email(r.email), address(r.address), isDeleted(r.isDeleted) {}

// Getters
int Customer::getId() const { return id; }
std::string Customer::getName() const { return name; }
std::string Customer::getPhone() const { return phone; }
std::string Customer::getEmail() const { return email; }
std::string Customer::getAddress() const { return address; }
bool Customer::getIsDeleted() const { return isDeleted; }

// Setters
void Customer::setId(int id) { this->id = id; }
void Customer::setName(const std::string& name) { this->name = name; }
void Customer::setPhone(const std::string& phone) { this->phone = phone; }
void Customer::setEmail(const std::string& email) { this->email = email; }
void Customer::setAddress(const std::string& address) { this->address = address; }
void Customer::setIsDeleted(bool isDeleted) { this->isDeleted = isDeleted; }

CustomerRecord Customer::toRecord() const {
    CustomerRecord r;
    r.id = id;
    r.isDeleted = isDeleted;

    std::memset(r.name, 0, sizeof(r.name));
    std::memset(r.phone, 0, sizeof(r.phone));
    std::memset(r.email, 0, sizeof(r.email));
    std::memset(r.address, 0, sizeof(r.address));

    name.copy(r.name, sizeof(r.name) - 1);
    phone.copy(r.phone, sizeof(r.phone) - 1);
    email.copy(r.email, sizeof(r.email) - 1);
    address.copy(r.address, sizeof(r.address) - 1);

    return r;
}

int Customer::getNextId() {
    std::ifstream file(CUSTOMER_FILE, std::ios::binary);
    if (!file) return 1;

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    if (fileSize == 0) return 1;

    int totalRecords = fileSize / sizeof(CustomerRecord);
    file.seekg((totalRecords - 1) * sizeof(CustomerRecord));

    CustomerRecord r;
    if (file.read(reinterpret_cast<char*>(&r), sizeof(CustomerRecord))) {
        return r.id + 1;
    }
    return 1;
}

bool Customer::findById(int id, Customer& customer) {
    std::ifstream file(CUSTOMER_FILE, std::ios::binary);
    if (!file) return false;

    CustomerRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(CustomerRecord))) {
        if (r.id == id && !r.isDeleted) {
            customer = Customer(r);
            return true;
        }
    }
    return false;
}

bool Customer::findByPhone(const std::string& phone, Customer& customer) {
    std::ifstream file(CUSTOMER_FILE, std::ios::binary);
    if (!file) return false;

    CustomerRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(CustomerRecord))) {
        if (std::string(r.phone) == phone && !r.isDeleted) {
            customer = Customer(r);
            return true;
        }
    }
    return false;
}

std::vector<Customer> Customer::getAllActiveCustomers() {
    std::vector<Customer> list;
    std::ifstream file(CUSTOMER_FILE, std::ios::binary);
    if (!file) return list;

    CustomerRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(CustomerRecord))) {
        if (!r.isDeleted) {
            list.push_back(Customer(r));
        }
    }
    return list;
}

void Customer::displayCard() const {
    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "  Customer Details:\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  --------------------------------------------------\n";
    std::cout << "  Customer ID : " << id << "\n";
    std::cout << "  Name        : " << name << "\n";
    std::cout << "  Phone       : " << phone << "\n";
    std::cout << "  Email       : " << email << "\n";
    std::cout << "  Address     : " << address << "\n";
    std::cout << "  --------------------------------------------------\n";
    Utils::resetColor();
}

void Customer::addCustomerUI() {
    Utils::printHeader("ADD CUSTOMER");

    int id = getNextId();
    std::cout << "  Generating Customer ID: " << id << "\n\n";

    std::string name = Utils::getNonEmptyString("  Enter Customer Name: ");
    while (!Utils::isValidName(name)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Name must contain only alphabets and spaces.\n";
        Utils::resetColor();
        name = Utils::getNonEmptyString("  Enter Customer Name: ");
    }

    std::string phone = Utils::getValidatedPhone("  Enter Phone Number: ");
    Customer temp;
    if (findByPhone(phone, temp)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Error: A customer with this phone number already exists!\n";
        temp.displayCard();
        Utils::pressEnterToContinue();
        return;
    }

    std::string email = Utils::getValidatedEmail("  Enter Email Address: ");
    std::string address = Utils::getNonEmptyString("  Enter Address: ");

    Customer newCust(id, name, phone, email, address);
    CustomerRecord record = newCust.toRecord();

    std::ofstream file(CUSTOMER_FILE, std::ios::binary | std::ios::app);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file for writing.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    file.write(reinterpret_cast<const char*>(&record), sizeof(CustomerRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Customer added successfully!\n";
    newCust.displayCard();
    Utils::pressEnterToContinue();
}

void Customer::displayAllCustomersUI() {
    Utils::printHeader("CUSTOMER DIRECTORY");

    std::vector<Customer> customers = getAllActiveCustomers();
    if (customers.empty()) {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "  No customers found in the system.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    // Pagination constants
    const size_t pageSize = 5;
    size_t totalPages = (customers.size() + pageSize - 1) / pageSize;
    size_t currentPage = 0;

    while (true) {
        Utils::printHeader("CUSTOMER DIRECTORY (Page " + std::to_string(currentPage + 1) + " of " + std::to_string(totalPages) + ")");
        
        // Print Table Header
        Utils::setColor(Utils::Color::BOLD_WHITE);
        std::cout << "  "
                  << Utils::padString("ID", 5) << " | "
                  << Utils::padString("Name", 20) << " | "
                  << Utils::padString("Phone", 15) << " | "
                  << Utils::padString("Email", 25) << "\n";
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  ----------------------------------------------------------------------------\n";
        Utils::resetColor();

        size_t startIdx = currentPage * pageSize;
        size_t endIdx = std::min(startIdx + pageSize, customers.size());

        for (size_t i = startIdx; i < endIdx; ++i) {
            std::cout << "  "
                      << Utils::padString(std::to_string(customers[i].getId()), 5) << " | "
                      << Utils::padString(customers[i].getName(), 20) << " | "
                      << Utils::padString(customers[i].getPhone(), 15) << " | "
                      << Utils::padString(customers[i].getEmail(), 25) << "\n";
        }
        
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  ----------------------------------------------------------------------------\n";
        Utils::resetColor();

        std::cout << "  [N] Next Page  |  [P] Previous Page  |  [Q] Exit Directory\n";
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

void Customer::searchCustomerUI() {
    Utils::printHeader("SEARCH CUSTOMER");
    std::cout << "  1. Search by Customer ID\n";
    std::cout << "  2. Search by Phone Number\n";
    std::cout << "  3. Search by Name (Partial Match)\n";
    std::cout << "  4. Back\n\n";

    int choice = Utils::getValidatedInt("  Enter Choice (1-4): ", 1, 4);
    if (choice == 4) return;

    if (choice == 1) {
        int targetId = Utils::getValidatedInt("  Enter Customer ID: ");
        Customer c;
        if (findById(targetId, c)) {
            std::cout << "\n";
            c.displayCard();
        } else {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  Customer with ID " << targetId << " not found.\n";
            Utils::resetColor();
        }
    } else if (choice == 2) {
        std::string phone = Utils::getValidatedPhone("  Enter Phone Number: ");
        Customer c;
        if (findByPhone(phone, c)) {
            std::cout << "\n";
            c.displayCard();
        } else {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  Customer with phone number " << phone << " not found.\n";
            Utils::resetColor();
        }
    } else if (choice == 3) {
        std::string searchName = Utils::getNonEmptyString("  Enter Name or part of Name: ");
        searchName = Utils::toLowerCase(searchName);
        std::vector<Customer> list = getAllActiveCustomers();
        std::vector<Customer> results;

        for (const auto& c : list) {
            std::string currentName = Utils::toLowerCase(c.getName());
            if (currentName.find(searchName) != std::string::npos) {
                results.push_back(c);
            }
        }

        if (results.empty()) {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  No customers match the name \"" << searchName << "\".\n";
            Utils::resetColor();
        } else {
            std::cout << "\n  Found " << results.size() << " match(es):\n\n";
            for (const auto& c : results) {
                c.displayCard();
                std::cout << "\n";
            }
        }
    }
    Utils::pressEnterToContinue();
}

void Customer::updateCustomerUI() {
    Utils::printHeader("UPDATE CUSTOMER");
    int targetId = Utils::getValidatedInt("  Enter Customer ID to update: ");

    std::fstream file(CUSTOMER_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    CustomerRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(CustomerRecord))) {
        if (r.id == targetId && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Customer with ID " << targetId << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    Customer current(r);
    std::cout << "\n  Current Info:\n";
    current.displayCard();

    std::cout << "  Enter new details (leave blank to keep current):\n\n";
    
    std::string newName;
    while (true) {
        std::cout << "  New Name [" << current.getName() << "]: ";
        std::getline(std::cin, newName);
        newName = Utils::trim(newName);
        if (newName.empty()) {
            newName = current.getName();
            break;
        }
        if (Utils::isValidName(newName)) {
            break;
        }
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Name must contain only alphabets and spaces.\n";
        Utils::resetColor();
    }

    std::string newPhone;
    while (true) {
        std::cout << "  New Phone [" << current.getPhone() << "]: ";
        std::getline(std::cin, newPhone);
        newPhone = Utils::trim(newPhone);
        if (newPhone.empty()) {
            newPhone = current.getPhone();
            break;
        }
        if (Utils::isValidPhone(newPhone)) {
            // Check uniqueness if phone is changed
            Customer temp;
            if (newPhone != current.getPhone() && findByPhone(newPhone, temp)) {
                Utils::setColor(Utils::Color::RED);
                std::cout << "  Error: A customer with this phone number already exists.\n";
                Utils::resetColor();
                continue;
            }
            break;
        }
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Invalid phone number. Must be 10-12 digits.\n";
        Utils::resetColor();
    }

    std::string newEmail;
    while (true) {
        std::cout << "  New Email [" << current.getEmail() << "]: ";
        std::getline(std::cin, newEmail);
        newEmail = Utils::trim(newEmail);
        if (newEmail.empty()) {
            newEmail = current.getEmail();
            break;
        }
        if (Utils::isValidEmail(newEmail)) {
            break;
        }
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Invalid email format.\n";
        Utils::resetColor();
    }

    std::string newAddress;
    std::cout << "  New Address [" << current.getAddress() << "]: ";
    std::getline(std::cin, newAddress);
    newAddress = Utils::trim(newAddress);
    if (newAddress.empty()) {
        newAddress = current.getAddress();
    }

    Customer updatedCust(targetId, newName, newPhone, newEmail, newAddress);
    CustomerRecord updatedRecord = updatedCust.toRecord();

    file.seekp(recordIndex * sizeof(CustomerRecord));
    file.write(reinterpret_cast<const char*>(&updatedRecord), sizeof(CustomerRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Customer updated successfully!\n";
    updatedCust.displayCard();
    Utils::pressEnterToContinue();
}

void Customer::deleteCustomerUI() {
    Utils::printHeader("DELETE CUSTOMER");
    int targetId = Utils::getValidatedInt("  Enter Customer ID to delete: ");

    std::fstream file(CUSTOMER_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    CustomerRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(CustomerRecord))) {
        if (r.id == targetId && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Customer with ID " << targetId << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    Customer target(r);
    std::cout << "\n";
    target.displayCard();

    bool confirm = Utils::getConfirmation("  Are you sure you want to delete this customer? All historical links will be preserved.");
    if (confirm) {
        r.isDeleted = true;
        file.seekp(recordIndex * sizeof(CustomerRecord));
        file.write(reinterpret_cast<const char*>(&r), sizeof(CustomerRecord));
        
        Utils::setColor(Utils::Color::GREEN);
        std::cout << "\n  Customer deleted successfully.\n";
        Utils::resetColor();
    } else {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "\n  Deletion cancelled.\n";
        Utils::resetColor();
    }

    file.close();
    Utils::pressEnterToContinue();
}
