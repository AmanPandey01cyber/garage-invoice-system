#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include <vector>

struct CustomerRecord {
    int id;
    char name[50];
    char phone[15];
    char email[50];
    char address[100];
    bool isDeleted;
};

class Customer {
private:
    int id;
    std::string name;
    std::string phone;
    std::string email;
    std::string address;
    bool isDeleted;

public:
    Customer();
    Customer(int id, const std::string& name, const std::string& phone, const std::string& email, const std::string& address, bool isDeleted = false);
    Customer(const CustomerRecord& record);

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getPhone() const;
    std::string getEmail() const;
    std::string getAddress() const;
    bool getIsDeleted() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setEmail(const std::string& email);
    void setAddress(const std::string& address);
    void setIsDeleted(bool isDeleted);

    // Record conversion
    CustomerRecord toRecord() const;

    // Static CRUD/Management Operations
    static void addCustomerUI();
    static void displayAllCustomersUI();
    static void searchCustomerUI();
    static void updateCustomerUI();
    static void deleteCustomerUI();
    
    // Static Query Helpers
    static int getNextId();
    static bool findById(int id, Customer& customer);
    static bool findByPhone(const std::string& phone, Customer& customer);
    static std::vector<Customer> getAllActiveCustomers();

    // UI print methods
    void displayCard() const;
};

#endif // CUSTOMER_H
