#include "Employee.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>

const std::string EMPLOYEE_FILE = "employees.dat";

Employee::Employee()
    : id(0), name(""), designation(""), salary(0.0), phone(""), isDeleted(false) {}

Employee::Employee(int id, const std::string& name, const std::string& designation, double salary, const std::string& phone, bool isDeleted)
    : id(id), name(name), designation(designation), salary(salary), phone(phone), isDeleted(isDeleted) {}

Employee::Employee(const EmployeeRecord& r)
    : id(r.id), name(r.name), designation(r.designation), salary(r.salary), phone(r.phone), isDeleted(r.isDeleted) {}

// Getters
int Employee::getId() const { return id; }
std::string Employee::getName() const { return name; }
std::string Employee::getDesignation() const { return designation; }
double Employee::getSalary() const { return salary; }
std::string Employee::getPhone() const { return phone; }
bool Employee::getIsDeleted() const { return isDeleted; }

// Setters
void Employee::setId(int id) { this->id = id; }
void Employee::setName(const std::string& name) { this->name = name; }
void Employee::setDesignation(const std::string& designation) { this->designation = designation; }
void Employee::setSalary(double salary) { this->salary = salary; }
void Employee::setPhone(const std::string& phone) { this->phone = phone; }
void Employee::setIsDeleted(bool isDeleted) { this->isDeleted = isDeleted; }

EmployeeRecord Employee::toRecord() const {
    EmployeeRecord r;
    r.id = id;
    r.salary = salary;
    r.isDeleted = isDeleted;

    std::memset(r.name, 0, sizeof(r.name));
    std::memset(r.designation, 0, sizeof(r.designation));
    std::memset(r.phone, 0, sizeof(r.phone));

    name.copy(r.name, sizeof(r.name) - 1);
    designation.copy(r.designation, sizeof(r.designation) - 1);
    phone.copy(r.phone, sizeof(r.phone) - 1);

    return r;
}

int Employee::getNextId() {
    std::ifstream file(EMPLOYEE_FILE, std::ios::binary);
    if (!file) return 1;

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    if (fileSize == 0) return 1;

    int totalRecords = fileSize / sizeof(EmployeeRecord);
    file.seekg((totalRecords - 1) * sizeof(EmployeeRecord));

    EmployeeRecord r;
    if (file.read(reinterpret_cast<char*>(&r), sizeof(EmployeeRecord))) {
        return r.id + 1;
    }
    return 1;
}

bool Employee::findById(int id, Employee& emp) {
    std::ifstream file(EMPLOYEE_FILE, std::ios::binary);
    if (!file) return false;

    EmployeeRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(EmployeeRecord))) {
        if (r.id == id && !r.isDeleted) {
            emp = Employee(r);
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

std::vector<Employee> Employee::getAllActiveEmployees() {
    std::vector<Employee> list;
    std::ifstream file(EMPLOYEE_FILE, std::ios::binary);
    if (!file) return list;

    EmployeeRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(EmployeeRecord))) {
        if (!r.isDeleted) {
            list.push_back(Employee(r));
        }
    }
    file.close();
    return list;
}

void Employee::displayCard() const {
    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "  Employee Details:\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  --------------------------------------------------\n";
    std::cout << "  Employee ID : " << id << "\n";
    std::cout << "  Name        : " << name << "\n";
    std::cout << "  Designation : " << designation << "\n";
    std::cout << "  Salary (Rs) : " << std::fixed << std::setprecision(2) << salary << "\n";
    std::cout << "  Phone       : " << phone << "\n";
    std::cout << "  --------------------------------------------------\n";
    Utils::resetColor();
}

void Employee::addEmployeeUI() {
    Utils::printHeader("ADD EMPLOYEE");

    int id = getNextId();
    std::cout << "  Generating Employee ID: " << id << "\n\n";

    std::string name = Utils::getNonEmptyString("  Enter Employee Name: ");
    while (!Utils::isValidName(name)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Name must contain only alphabets and spaces.\n";
        Utils::resetColor();
        name = Utils::getNonEmptyString("  Enter Employee Name: ");
    }

    std::string designation = Utils::getNonEmptyString("  Enter Designation (e.g. Mechanic, Receptionist): ");
    double salary = Utils::getValidatedDouble("  Enter Salary: ", 0.0);
    std::string phone = Utils::getValidatedPhone("  Enter Phone Number: ");

    Employee newEmp(id, name, designation, salary, phone);
    EmployeeRecord record = newEmp.toRecord();

    std::ofstream file(EMPLOYEE_FILE, std::ios::binary | std::ios::app);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file for writing.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    file.write(reinterpret_cast<const char*>(&record), sizeof(EmployeeRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Employee added successfully!\n";
    newEmp.displayCard();
    Utils::pressEnterToContinue();
}

void Employee::displayAllEmployeesUI() {
    Utils::printHeader("EMPLOYEE DIRECTORY");

    std::vector<Employee> employees = getAllActiveEmployees();
    if (employees.empty()) {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "  No employees registered in the system.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    const size_t pageSize = 5;
    size_t totalPages = (employees.size() + pageSize - 1) / pageSize;
    size_t currentPage = 0;

    while (true) {
        Utils::printHeader("EMPLOYEE DIRECTORY (Page " + std::to_string(currentPage + 1) + " of " + std::to_string(totalPages) + ")");

        Utils::setColor(Utils::Color::BOLD_WHITE);
        std::cout << "  "
                  << Utils::padString("ID", 5) << " | "
                  << Utils::padString("Name", 20) << " | "
                  << Utils::padString("Designation", 20) << " | "
                  << Utils::padString("Salary", 12) << " | "
                  << Utils::padString("Phone", 12) << "\n";
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  --------------------------------------------------------------------------------\n";
        Utils::resetColor();

        size_t startIdx = currentPage * pageSize;
        size_t endIdx = std::min(startIdx + pageSize, employees.size());

        for (size_t i = startIdx; i < endIdx; ++i) {
            std::cout << "  "
                      << Utils::padString(std::to_string(employees[i].getId()), 5) << " | "
                      << Utils::padString(employees[i].getName(), 20) << " | "
                      << Utils::padString(employees[i].getDesignation(), 20) << " | "
                      << Utils::padString(std::to_string(int(employees[i].getSalary())), 12) << " | "
                      << Utils::padString(employees[i].getPhone(), 12) << "\n";
        }

        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  --------------------------------------------------------------------------------\n";
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

void Employee::searchEmployeeUI() {
    Utils::printHeader("SEARCH EMPLOYEE");
    std::cout << "  1. Search by Employee ID\n";
    std::cout << "  2. Search by Name (Partial Match)\n";
    std::cout << "  3. Back\n\n";

    int choice = Utils::getValidatedInt("  Enter Choice (1-3): ", 1, 3);
    if (choice == 3) return;

    if (choice == 1) {
        int targetId = Utils::getValidatedInt("  Enter Employee ID: ");
        Employee emp;
        if (findById(targetId, emp)) {
            std::cout << "\n";
            emp.displayCard();
        } else {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  Employee with ID " << targetId << " not found.\n";
            Utils::resetColor();
        }
    } else if (choice == 2) {
        std::string searchName = Utils::getNonEmptyString("  Enter Name or part of Name: ");
        searchName = Utils::toLowerCase(searchName);
        std::vector<Employee> list = getAllActiveEmployees();
        std::vector<Employee> results;

        for (const auto& emp : list) {
            std::string currentName = Utils::toLowerCase(emp.getName());
            if (currentName.find(searchName) != std::string::npos) {
                results.push_back(emp);
            }
        }

        if (results.empty()) {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  No employees match the name \"" << searchName << "\".\n";
            Utils::resetColor();
        } else {
            std::cout << "\n  Found " << results.size() << " match(es):\n\n";
            for (const auto& emp : results) {
                emp.displayCard();
                std::cout << "\n";
            }
        }
    }
    Utils::pressEnterToContinue();
}

void Employee::updateEmployeeUI() {
    Utils::printHeader("UPDATE EMPLOYEE");
    int targetId = Utils::getValidatedInt("  Enter Employee ID to update: ");

    std::fstream file(EMPLOYEE_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    EmployeeRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(EmployeeRecord))) {
        if (r.id == targetId && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Employee with ID " << targetId << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        file.close();
        return;
    }

    Employee current(r);
    std::cout << "\n  Current Info:\n";
    current.displayCard();

    std::cout << "  Enter new details (leave blank to keep current):\n\n";

    std::cout << "  New Name [" << current.getName() << "]: ";
    std::string nName;
    std::getline(std::cin, nName);
    nName = Utils::trim(nName);
    while (!nName.empty() && !Utils::isValidName(nName)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Name must contain only alphabets and spaces.\n";
        Utils::resetColor();
        std::cout << "  New Name [" << current.getName() << "]: ";
        std::getline(std::cin, nName);
        nName = Utils::trim(nName);
    }
    if (nName.empty()) nName = current.getName();

    std::cout << "  New Designation [" << current.getDesignation() << "]: ";
    std::string nDesig;
    std::getline(std::cin, nDesig);
    nDesig = Utils::trim(nDesig);
    if (nDesig.empty()) nDesig = current.getDesignation();

    std::string nSalaryStr;
    double nSalary = current.getSalary();
    while (true) {
        std::cout << "  New Salary [" << current.getSalary() << "]: ";
        std::getline(std::cin, nSalaryStr);
        nSalaryStr = Utils::trim(nSalaryStr);
        if (nSalaryStr.empty()) {
            break;
        }
        try {
            double val = std::stod(nSalaryStr);
            if (val >= 0) {
                nSalary = val;
                break;
            }
        } catch (...) {}
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Salary must be a positive decimal.\n";
        Utils::resetColor();
    }

    std::cout << "  New Phone [" << current.getPhone() << "]: ";
    std::string nPhone;
    std::getline(std::cin, nPhone);
    nPhone = Utils::trim(nPhone);
    while (!nPhone.empty() && !Utils::isValidPhone(nPhone)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Invalid phone number.\n";
        Utils::resetColor();
        std::cout << "  New Phone [" << current.getPhone() << "]: ";
        std::getline(std::cin, nPhone);
        nPhone = Utils::trim(nPhone);
    }
    if (nPhone.empty()) nPhone = current.getPhone();

    Employee updatedEmp(targetId, nName, nDesig, nSalary, nPhone);
    EmployeeRecord updatedRecord = updatedEmp.toRecord();

    file.seekp(recordIndex * sizeof(EmployeeRecord));
    file.write(reinterpret_cast<const char*>(&updatedRecord), sizeof(EmployeeRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Employee details updated successfully!\n";
    updatedEmp.displayCard();
    Utils::pressEnterToContinue();
}

void Employee::deleteEmployeeUI() {
    Utils::printHeader("DELETE EMPLOYEE");
    int targetId = Utils::getValidatedInt("  Enter Employee ID to delete: ");

    std::fstream file(EMPLOYEE_FILE, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open database file.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    EmployeeRecord r;
    int recordIndex = 0;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&r), sizeof(EmployeeRecord))) {
        if (r.id == targetId && !r.isDeleted) {
            found = true;
            break;
        }
        recordIndex++;
    }

    if (!found) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Employee with ID " << targetId << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        file.close();
        return;
    }

    Employee target(r);
    std::cout << "\n";
    target.displayCard();

    bool confirm = Utils::getConfirmation("  Are you sure you want to delete this employee record?");
    if (confirm) {
        r.isDeleted = true;
        file.seekp(recordIndex * sizeof(EmployeeRecord));
        file.write(reinterpret_cast<const char*>(&r), sizeof(EmployeeRecord));

        Utils::setColor(Utils::Color::GREEN);
        std::cout << "\n  Employee deleted successfully.\n";
        Utils::resetColor();
    } else {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "\n  Deletion cancelled.\n";
        Utils::resetColor();
    }

    file.close();
    Utils::pressEnterToContinue();
}
