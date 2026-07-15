#ifndef INVOICE_H
#define INVOICE_H

#include <string>
#include <vector>
#include <utility>
#include "Payment.h"

struct InvoiceRecord {
    int invoiceNumber;
    int customerId;
    char customerName[50];
    char customerPhone[15];
    char vehicleNumber[20];
    char vehicleBrand[30];
    char vehicleModel[30];
    char date[15];
    
    int serviceIds[5];
    int serviceCount;
    
    int partIds[5];
    int partQtys[5];
    int partCount;
    
    double labourCharges;
    double partsCharges;
    double subtotal;
    double discount;
    double gstAmount;
    double grandTotal;
    
    bool isPaid;
    int paymentMode; 
    char paymentRef[50];
    bool isDeleted;
};

class Invoice {
private:
    int invoiceNumber;
    int customerId;
    std::string customerName;
    std::string customerPhone;
    std::string vehicleNumber;
    std::string vehicleBrand;
    std::string vehicleModel;
    std::string date;
    
    std::vector<int> serviceIds;
    std::vector<std::pair<int, int>> partsList; // pair<PartId, Quantity>
    
    double labourCharges;
    double partsCharges;
    double subtotal;
    double discount;
    double gstAmount;
    double grandTotal;
    
    bool isPaid;
    Payment::Mode paymentMode;
    std::string paymentRef;
    bool isDeleted;

public:
    Invoice();
    Invoice(const InvoiceRecord& record);

    // Getters
    int getInvoiceNumber() const;
    int getCustomerId() const;
    std::string getCustomerName() const;
    std::string getCustomerPhone() const;
    std::string getVehicleNumber() const;
    std::string getVehicleBrand() const;
    std::string getVehicleModel() const;
    std::string getDate() const;
    std::vector<int> getServiceIds() const;
    std::vector<std::pair<int, int>> getPartsList() const;
    double getLabourCharges() const;
    double getPartsCharges() const;
    double getSubtotal() const;
    double getDiscount() const;
    double getGstAmount() const;
    double getGrandTotal() const;
    bool getIsPaid() const;
    Payment::Mode getPaymentMode() const;
    std::string getPaymentRef() const;
    bool getIsDeleted() const;

    // Setters / Updates
    void setPaid(bool paid, Payment::Mode mode, const std::string& ref);

    // Record conversion
    InvoiceRecord toRecord() const;

    // Static CRUD/Management Operations
    static void createInvoiceUI();
    static void searchInvoiceUI();
    static void viewInvoiceHistoryUI();

    // Static Query Helpers
    static int getNextInvoiceNumber();
    static bool findByInvoiceNumber(int number, Invoice& invoice);
    static std::vector<Invoice> findByCustomerId(int customerId);
    static std::vector<Invoice> findByVehicleNumber(const std::string& vehNo);
    static std::vector<Invoice> getAllActiveInvoices();

    // Output Printer
    void printInvoiceCard() const;
};

#endif // INVOICE_H
