#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
#include <vector>

struct EmployeeRecord {
    int id;
    char name[50];
    char designation[30];
    double salary;
    char phone[15];
    bool isDeleted;
};

class Employee {
private:
    int id;
    std::string name;
    std::string designation;
    double salary;
    std::string phone;
    bool isDeleted;

public:
    Employee();
    Employee(int id, const std::string& name, const std::string& designation, double salary, const std::string& phone, bool isDeleted = false);
    Employee(const EmployeeRecord& record);

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getDesignation() const;
    double getSalary() const;
    std::string getPhone() const;
    bool getIsDeleted() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setDesignation(const std::string& designation);
    void setSalary(double salary);
    void setPhone(const std::string& phone);
    void setIsDeleted(bool isDeleted);

    // Record conversion
    EmployeeRecord toRecord() const;

    // Static CRUD/Management Operations
    static void addEmployeeUI();
    static void displayAllEmployeesUI();
    static void searchEmployeeUI();
    static void updateEmployeeUI();
    static void deleteEmployeeUI();

    // Static Query Helpers
    static int getNextId();
    static bool findById(int id, Employee& emp);
    static std::vector<Employee> getAllActiveEmployees();

    // UI display helpers
    void displayCard() const;
};

#endif // EMPLOYEE_H
