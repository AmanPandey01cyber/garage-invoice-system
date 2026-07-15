#include "Invoice.h"
#include "Customer.h"
#include "Vehicle.h"
#include "Service.h"
#include "Part.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <algorithm>

const std::string INVOICE_FILE = "invoices.dat";

Invoice::Invoice()
    : invoiceNumber(0), customerId(0), customerName(""), customerPhone(""),
      vehicleNumber(""), vehicleBrand(""), vehicleModel(""), date(""),
      labourCharges(0.0), partsCharges(0.0), subtotal(0.0), discount(0.0),
      gstAmount(0.0), grandTotal(0.0), isPaid(false),
      paymentMode(Payment::Mode::CASH), paymentRef("N/A"), isDeleted(false) {}

Invoice::Invoice(const InvoiceRecord& r)
    : invoiceNumber(r.invoiceNumber), customerId(r.customerId), customerName(r.customerName),
      customerPhone(r.customerPhone), vehicleNumber(r.vehicleNumber), vehicleBrand(r.vehicleBrand),
      vehicleModel(r.vehicleModel), date(r.date), labourCharges(r.labourCharges),
      partsCharges(r.partsCharges), subtotal(r.subtotal), discount(r.discount),
      gstAmount(r.gstAmount), grandTotal(r.grandTotal), isPaid(r.isPaid),
      paymentMode(static_cast<Payment::Mode>(r.paymentMode)), paymentRef(r.paymentRef), isDeleted(r.isDeleted) {
    
    for (int i = 0; i < r.serviceCount; ++i) {
        serviceIds.push_back(r.serviceIds[i]);
    }
    for (int i = 0; i < r.partCount; ++i) {
        partsList.push_back({r.partIds[i], r.partQtys[i]});
    }
}

// Getters
int Invoice::getInvoiceNumber() const { return invoiceNumber; }
int Invoice::getCustomerId() const { return customerId; }
std::string Invoice::getCustomerName() const { return customerName; }
std::string Invoice::getCustomerPhone() const { return customerPhone; }
std::string Invoice::getVehicleNumber() const { return vehicleNumber; }
std::string Invoice::getVehicleBrand() const { return vehicleBrand; }
std::string Invoice::getVehicleModel() const { return vehicleModel; }
std::string Invoice::getDate() const { return date; }
std::vector<int> Invoice::getServiceIds() const { return serviceIds; }
std::vector<std::pair<int, int>> Invoice::getPartsList() const { return partsList; }
double Invoice::getLabourCharges() const { return labourCharges; }
double Invoice::getPartsCharges() const { return partsCharges; }
double Invoice::getSubtotal() const { return subtotal; }
double Invoice::getDiscount() const { return discount; }
double Invoice::getGstAmount() const { return gstAmount; }
double Invoice::getGrandTotal() const { return grandTotal; }
bool Invoice::getIsPaid() const { return isPaid; }
Payment::Mode Invoice::getPaymentMode() const { return paymentMode; }
std::string Invoice::getPaymentRef() const { return paymentRef; }
bool Invoice::getIsDeleted() const { return isDeleted; }

void Invoice::setPaid(bool paid, Payment::Mode mode, const std::string& ref) {
    isPaid = paid;
    paymentMode = mode;
    paymentRef = ref;
}

InvoiceRecord Invoice::toRecord() const {
    InvoiceRecord r;
    r.invoiceNumber = invoiceNumber;
    r.customerId = customerId;
    r.labourCharges = labourCharges;
    r.partsCharges = partsCharges;
    r.subtotal = subtotal;
    r.discount = discount;
    r.gstAmount = gstAmount;
    r.grandTotal = grandTotal;
    r.isPaid = isPaid;
    r.paymentMode = static_cast<int>(paymentMode);
    r.isDeleted = isDeleted;

    std::memset(r.customerName, 0, sizeof(r.customerName));
    std::memset(r.customerPhone, 0, sizeof(r.customerPhone));
    std::memset(r.vehicleNumber, 0, sizeof(r.vehicleNumber));
    std::memset(r.vehicleBrand, 0, sizeof(r.vehicleBrand));
    std::memset(r.vehicleModel, 0, sizeof(r.vehicleModel));
    std::memset(r.date, 0, sizeof(r.date));
    std::memset(r.paymentRef, 0, sizeof(r.paymentRef));

    customerName.copy(r.customerName, sizeof(r.customerName) - 1);
    customerPhone.copy(r.customerPhone, sizeof(r.customerPhone) - 1);
    vehicleNumber.copy(r.vehicleNumber, sizeof(r.vehicleNumber) - 1);
    vehicleBrand.copy(r.vehicleBrand, sizeof(r.vehicleBrand) - 1);
    vehicleModel.copy(r.vehicleModel, sizeof(r.vehicleModel) - 1);
    date.copy(r.date, sizeof(r.date) - 1);
    paymentRef.copy(r.paymentRef, sizeof(r.paymentRef) - 1);

    // Pack services
    r.serviceCount = std::min(static_cast<int>(serviceIds.size()), 5);
    std::memset(r.serviceIds, 0, sizeof(r.serviceIds));
    for (int i = 0; i < r.serviceCount; ++i) {
        r.serviceIds[i] = serviceIds[i];
    }

    // Pack parts
    r.partCount = std::min(static_cast<int>(partsList.size()), 5);
    std::memset(r.partIds, 0, sizeof(r.partIds));
    std::memset(r.partQtys, 0, sizeof(r.partQtys));
    for (int i = 0; i < r.partCount; ++i) {
        r.partIds[i] = partsList[i].first;
        r.partQtys[i] = partsList[i].second;
    }

    return r;
}

int Invoice::getNextInvoiceNumber() {
    std::ifstream file(INVOICE_FILE, std::ios::binary);
    if (!file) return 1001; // Invoices start at 1001

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    if (fileSize == 0) return 1001;

    int totalRecords = fileSize / sizeof(InvoiceRecord);
    file.seekg((totalRecords - 1) * sizeof(InvoiceRecord));

    InvoiceRecord r;
    if (file.read(reinterpret_cast<char*>(&r), sizeof(InvoiceRecord))) {
        return r.invoiceNumber + 1;
    }
    return 1001;
}

bool Invoice::findByInvoiceNumber(int number, Invoice& invoice) {
    std::ifstream file(INVOICE_FILE, std::ios::binary);
    if (!file) return false;

    InvoiceRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(InvoiceRecord))) {
        if (r.invoiceNumber == number && !r.isDeleted) {
            invoice = Invoice(r);
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

std::vector<Invoice> Invoice::findByCustomerId(int customerId) {
    std::vector<Invoice> list;
    std::ifstream file(INVOICE_FILE, std::ios::binary);
    if (!file) return list;

    InvoiceRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(InvoiceRecord))) {
        if (r.customerId == customerId && !r.isDeleted) {
            list.push_back(Invoice(r));
        }
    }
    file.close();
    return list;
}

std::vector<Invoice> Invoice::findByVehicleNumber(const std::string& vehNo) {
    std::vector<Invoice> list;
    std::ifstream file(INVOICE_FILE, std::ios::binary);
    if (!file) return list;

    std::string searchNum = Utils::trim(Utils::toLowerCase(vehNo));
    InvoiceRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(InvoiceRecord))) {
        std::string recordNum = Utils::trim(Utils::toLowerCase(r.vehicleNumber));
        if (recordNum == searchNum && !r.isDeleted) {
            list.push_back(Invoice(r));
        }
    }
    file.close();
    return list;
}

std::vector<Invoice> Invoice::getAllActiveInvoices() {
    std::vector<Invoice> list;
    std::ifstream file(INVOICE_FILE, std::ios::binary);
    if (!file) return list;

    InvoiceRecord r;
    while (file.read(reinterpret_cast<char*>(&r), sizeof(InvoiceRecord))) {
        if (!r.isDeleted) {
            list.push_back(Invoice(r));
        }
    }
    file.close();
    return list;
}

void Invoice::printInvoiceCard() const {
    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "                  GARAGE INVOICE                        \n";
    std::cout << "--------------------------------------------------------\n";
    Utils::resetColor();
    std::cout << "Invoice No : " << invoiceNumber << "\n";
    std::cout << "Date       : " << date << "\n\n";

    std::cout << "Customer   : " << customerName << " (Phone: " << customerPhone << ")\n";
    std::cout << "Vehicle    : " << vehicleBrand << " " << vehicleModel << " (" << vehicleNumber << ")\n\n";

    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "Services\n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << Utils::padString("Service Name", 30) << " " 
              << Utils::padString("Qty", 8, ' ', true) << " " 
              << Utils::padString("Price (Rs)", 14, ' ', true) << "\n";
    std::cout << "--------------------------------------------------------\n";
    Utils::resetColor();

    for (int sId : serviceIds) {
        Service srv;
        if (Service::findById(sId, srv)) {
            std::cout << Utils::padString(srv.getName(), 30) << " " 
                      << Utils::padString("1", 8, ' ', true) << " " 
                      << Utils::padString(std::to_string(int(srv.getLabourCost())), 14, ' ', true) << "\n";
        } else {
            std::cout << Utils::padString("Unknown Service (ID:" + std::to_string(sId) + ")", 30) << " " 
                      << Utils::padString("1", 8, ' ', true) << " " 
                      << Utils::padString("0", 14, ' ', true) << "\n";
        }
    }

    std::cout << "\n";
    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "Parts\n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << Utils::padString("Part Name", 30) << " " 
              << Utils::padString("Qty", 8, ' ', true) << " " 
              << Utils::padString("Price (Rs)", 14, ' ', true) << "\n";
    std::cout << "--------------------------------------------------------\n";
    Utils::resetColor();

    for (const auto& item : partsList) {
        Part p;
        if (Part::findById(item.first, p)) {
            std::cout << Utils::padString(p.getName(), 30) << " " 
                      << Utils::padString(std::to_string(item.second), 8, ' ', true) << " " 
                      << Utils::padString(std::to_string(int(p.getUnitPrice() * item.second)), 14, ' ', true) << "\n";
        } else {
            std::cout << Utils::padString("Unknown Part (ID:" + std::to_string(item.first) + ")", 30) << " " 
                      << Utils::padString(std::to_string(item.second), 8, ' ', true) << " " 
                      << Utils::padString("0", 14, ' ', true) << "\n";
        }
    }

    Utils::setColor(Utils::Color::CYAN);
    std::cout << "--------------------------------------------------------\n";
    Utils::resetColor();
    std::cout << "Labour Charge   : Rs " << std::fixed << std::setprecision(2) << labourCharges << "\n";
    std::cout << "Parts Charge    : Rs " << partsCharges << "\n";
    std::cout << "Subtotal        : Rs " << subtotal << "\n";
    std::cout << "Discount        : Rs " << discount << "\n";
    std::cout << "GST (18%)       : Rs " << gstAmount << "\n";
    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "Grand Total     : Rs " << grandTotal << "\n";
    Utils::resetColor();
    std::cout << "Payment Mode    : " << Payment::modeToString(paymentMode) << "\n";
    std::cout << "Payment Status  : ";
    if (isPaid) {
        Utils::setColor(Utils::Color::GREEN);
        std::cout << "Paid\n";
        Utils::resetColor();
        std::cout << "Ref Number      : " << paymentRef << "\n";
    } else {
        Utils::setColor(Utils::Color::RED);
        std::cout << "Unpaid\n";
        Utils::resetColor();
    }
    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "--------------------------------------------------------\n";
    std::cout << "                 Thank You for Visiting                 \n";
    std::cout << "--------------------------------------------------------\n";
    Utils::resetColor();
}

void Invoice::createInvoiceUI() {
    Utils::printHeader("CREATE INVOICE");

    std::string rawVehNo = Utils::getNonEmptyString("  Enter Vehicle Number: ");
    std::string vehNo = Utils::trim(rawVehNo);
    std::transform(vehNo.begin(), vehNo.end(), vehNo.begin(), ::toupper);

    Vehicle veh;
    if (!Vehicle::findByNumber(vehNo, veh)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Error: Vehicle " << vehNo << " is not registered in the system!\n";
        std::cout << "  Please register the vehicle first.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    Customer cust;
    if (!Customer::findById(veh.getCustomerId(), cust)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Error: Associated Customer Owner ID " << veh.getCustomerId() << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    // Display billing overview
    std::cout << "\n  --- Billing Customer ---\n";
    cust.displayCard();
    std::cout << "\n  --- Serviced Vehicle ---\n";
    veh.displayCard();
    std::cout << "\n";

    std::vector<int> selectedServices;
    std::vector<std::pair<int, int>> selectedParts;
    double labourSum = 0.0;
    double partsSum = 0.0;

    // 1. Add Services Loop
    while (selectedServices.size() < 5) {
        Utils::printHeader("SELECT SERVICES (" + std::to_string(selectedServices.size()) + "/5 Selected)");
        
        // Show short active list
        std::vector<Service> activeServices = Service::getAllActiveServices();
        for (const auto& s : activeServices) {
            std::cout << "  " << std::setw(3) << s.getId() << ". " << Utils::padString(s.getName(), 25) << " (Rs " << int(s.getLabourCost()) << ")\n";
        }
        std::cout << "  0. Stop Adding Services\n\n";

        int sId = Utils::getValidatedInt("  Select Service ID: ", 0, 1000);
        if (sId == 0) {
            if (selectedServices.empty()) {
                Utils::setColor(Utils::Color::RED);
                std::cout << "  Error: You must add at least one service to create an invoice.\n";
                Utils::resetColor();
                Utils::pressEnterToContinue();
                continue;
            }
            break;
        }

        Service temp;
        if (Service::findById(sId, temp)) {
            // Check if already added
            if (std::find(selectedServices.begin(), selectedServices.end(), sId) != selectedServices.end()) {
                Utils::setColor(Utils::Color::YELLOW);
                std::cout << "  Service already added to invoice!\n";
                Utils::resetColor();
                Utils::pressEnterToContinue();
                continue;
            }
            selectedServices.push_back(sId);
            labourSum += temp.getLabourCost();
            std::cout << "  Service \"" << temp.getName() << "\" added.\n";
            Utils::sleepMs(500);
        } else {
            Utils::setColor(Utils::Color::RED);
            std::cout << "  Invalid Service ID.\n";
            Utils::resetColor();
            Utils::pressEnterToContinue();
        }
    }

    // 2. Add Spare Parts Loop
    while (selectedParts.size() < 5) {
        Utils::printHeader("SELECT SPARE PARTS (" + std::to_string(selectedParts.size()) + "/5 Selected)");
        std::vector<Part> activeParts = Part::getAllActiveParts();
        
        if (activeParts.empty()) {
            std::cout << "  (No spare parts available in inventory)\n";
            Utils::pressEnterToContinue();
            break;
        }

        for (const auto& p : activeParts) {
            std::cout << "  " << std::setw(3) << p.getId() << ". " << Utils::padString(p.getName(), 25) 
                      << " (Price: Rs " << int(p.getUnitPrice()) << " | Stock: " << p.getQuantity() << ")\n";
        }
        std::cout << "  0. Stop Adding Spare Parts\n\n";

        int pId = Utils::getValidatedInt("  Select Part ID: ", 0, 1000);
        if (pId == 0) break;

        Part temp;
        if (Part::findById(pId, temp)) {
            // Check if already added
            auto it = std::find_if(selectedParts.begin(), selectedParts.end(), 
                                   [pId](const std::pair<int,int>& p){ return p.first == pId; });
            if (it != selectedParts.end()) {
                Utils::setColor(Utils::Color::YELLOW);
                std::cout << "  Spare part already added to invoice!\n";
                Utils::resetColor();
                Utils::pressEnterToContinue();
                continue;
            }

            if (temp.getQuantity() <= 0) {
                Utils::setColor(Utils::Color::RED);
                std::cout << "  Error: Part is Out of Stock!\n";
                Utils::resetColor();
                Utils::pressEnterToContinue();
                continue;
            }

            int qty = Utils::getValidatedInt("  Enter Quantity: ", 1, temp.getQuantity());
            selectedParts.push_back({pId, qty});
            partsSum += (temp.getUnitPrice() * qty);
            std::cout << "  Part \"" << temp.getName() << "\" x " << qty << " added.\n";
            Utils::sleepMs(500);
        } else {
            Utils::setColor(Utils::Color::RED);
            std::cout << "  Invalid Part ID.\n";
            Utils::resetColor();
            Utils::pressEnterToContinue();
        }
    }

    // Calculators
    double sub = labourSum + partsSum;
    std::cout << "\n  Labour Charge Subtotal: Rs " << labourSum << "\n";
    std::cout << "  Spare Parts Subtotal  : Rs " << partsSum << "\n";
    std::cout << "  Gross Total           : Rs " << sub << "\n\n";

    double disc = Utils::getValidatedDouble("  Enter Discount Amount (Rs): ", 0.0, sub);
    double gst = (sub - disc) * 0.18; // 18% GST on discounted amount
    double grand = sub - disc + gst;

    std::cout << "  GST Amount (18%)      : Rs " << gst << "\n";
    std::cout << "  Grand Total Payable   : Rs " << grand << "\n\n";

    // Setup invoice object properties
    Invoice newInv;
    newInv.invoiceNumber = getNextInvoiceNumber();
    newInv.customerId = cust.getId();
    newInv.customerName = cust.getName();
    newInv.customerPhone = cust.getPhone();
    newInv.vehicleNumber = veh.getVehicleNumber();
    newInv.vehicleBrand = veh.getBrand();
    newInv.vehicleModel = veh.getModel();
    newInv.date = Utils::getCurrentDate();
    newInv.serviceIds = selectedServices;
    newInv.partsList = selectedParts;
    newInv.labourCharges = labourSum;
    newInv.partsCharges = partsSum;
    newInv.subtotal = sub;
    newInv.discount = disc;
    newInv.gstAmount = gst;
    newInv.grandTotal = grand;
    newInv.isDeleted = false;

    // Checkout
    bool payNow = Utils::getConfirmation("  Would you like to collect payment now?");
    if (payNow) {
        Payment::Mode mode = Payment::getPaymentModeUI();
        std::string ref;
        if (Payment::processPayment(grand, mode, ref)) {
            newInv.setPaid(true, mode, ref);
        } else {
            newInv.setPaid(false, Payment::Mode::CASH, "N/A");
        }
    } else {
        newInv.setPaid(false, Payment::Mode::CASH, "N/A");
    }

    // Save logic
    std::ofstream file(INVOICE_FILE, std::ios::binary | std::ios::app);
    if (!file) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Could not open invoice database for writing.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    // Commit inventory adjustments
    for (const auto& item : selectedParts) {
        Part::updateStock(item.first, item.second);
    }

    InvoiceRecord record = newInv.toRecord();
    file.write(reinterpret_cast<const char*>(&record), sizeof(InvoiceRecord));
    file.close();

    Utils::setColor(Utils::Color::GREEN);
    std::cout << "\n  Invoice successfully created and saved!\n";
    Utils::resetColor();
    
    // Print the professional layout
    newInv.printInvoiceCard();
    Utils::pressEnterToContinue();
}

void Invoice::searchInvoiceUI() {
    Utils::printHeader("SEARCH INVOICES");
    std::cout << "  1. Search by Invoice Number\n";
    std::cout << "  2. Search by Customer ID\n";
    std::cout << "  3. Search by Vehicle Number\n";
    std::cout << "  4. Back\n\n";

    int choice = Utils::getValidatedInt("  Enter choice (1-4): ", 1, 4);
    if (choice == 4) return;

    if (choice == 1) {
        int invNum = Utils::getValidatedInt("  Enter Invoice Number: ");
        Invoice inv;
        if (findByInvoiceNumber(invNum, inv)) {
            inv.printInvoiceCard();
            
            // Allow checking out if it was unpaid
            if (!inv.getIsPaid()) {
                bool pay = Utils::getConfirmation("  This invoice is UNPAID. Process payment now?");
                if (pay) {
                    Payment::Mode mode = Payment::getPaymentModeUI();
                    std::string ref;
                    if (Payment::processPayment(inv.getGrandTotal(), mode, ref)) {
                        // Open file in read/write, search and update
                        std::fstream file(INVOICE_FILE, std::ios::binary | std::ios::in | std::ios::out);
                        InvoiceRecord r;
                        int recordIdx = 0;
                        while (file.read(reinterpret_cast<char*>(&r), sizeof(InvoiceRecord))) {
                            if (r.invoiceNumber == invNum && !r.isDeleted) {
                                r.isPaid = true;
                                r.paymentMode = static_cast<int>(mode);
                                std::memset(r.paymentRef, 0, sizeof(r.paymentRef));
                                ref.copy(r.paymentRef, sizeof(r.paymentRef) - 1);

                                file.seekp(recordIdx * sizeof(InvoiceRecord));
                                file.write(reinterpret_cast<const char*>(&r), sizeof(InvoiceRecord));
                                break;
                            }
                            recordIdx++;
                        }
                        file.close();
                        
                        // Reload and print
                        findByInvoiceNumber(invNum, inv);
                        inv.printInvoiceCard();
                    }
                }
            }
        } else {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  Invoice not found.\n";
            Utils::resetColor();
        }
    } 
    else if (choice == 2) {
        int custId = Utils::getValidatedInt("  Enter Customer ID: ");
        std::vector<Invoice> list = findByCustomerId(custId);
        if (list.empty()) {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  No invoices found for Customer ID " << custId << ".\n";
            Utils::resetColor();
        } else {
            std::cout << "\n  Found " << list.size() << " invoice(s):\n";
            for (const auto& inv : list) {
                std::cout << "  Invoice #" << inv.getInvoiceNumber() << " | Date: " << inv.getDate() 
                          << " | Total: Rs " << int(inv.getGrandTotal()) << " | Status: " 
                          << (inv.getIsPaid() ? "Paid" : "Unpaid") << "\n";
            }
            std::cout << "\n";
            int printChoice = Utils::getValidatedInt("  Enter Invoice Number to view details (0 to exit): ", 0, 100000);
            if (printChoice > 0) {
                Invoice inv;
                if (findByInvoiceNumber(printChoice, inv) && inv.getCustomerId() == custId) {
                    inv.printInvoiceCard();
                } else {
                    Utils::setColor(Utils::Color::RED);
                    std::cout << "  Invoice not found or unauthorized.\n";
                    Utils::resetColor();
                }
            }
        }
    } 
    else if (choice == 3) {
        std::string rawVehNo = Utils::getNonEmptyString("  Enter Vehicle Number: ");
        std::string vehNo = Utils::trim(rawVehNo);
        std::vector<Invoice> list = findByVehicleNumber(vehNo);
        if (list.empty()) {
            Utils::setColor(Utils::Color::RED);
            std::cout << "\n  No invoices found for Vehicle " << vehNo << ".\n";
            Utils::resetColor();
        } else {
            std::cout << "\n  Found " << list.size() << " invoice(s):\n";
            for (const auto& inv : list) {
                std::cout << "  Invoice #" << inv.getInvoiceNumber() << " | Date: " << inv.getDate() 
                          << " | Total: Rs " << int(inv.getGrandTotal()) << " | Status: " 
                          << (inv.getIsPaid() ? "Paid" : "Unpaid") << "\n";
            }
            std::cout << "\n";
            int printChoice = Utils::getValidatedInt("  Enter Invoice Number to view details (0 to exit): ", 0, 100000);
            if (printChoice > 0) {
                Invoice inv;
                if (findByInvoiceNumber(printChoice, inv)) {
                    inv.printInvoiceCard();
                } else {
                    Utils::setColor(Utils::Color::RED);
                    std::cout << "  Invoice not found.\n";
                    Utils::resetColor();
                }
            }
        }
    }
    Utils::pressEnterToContinue();
}

void Invoice::viewInvoiceHistoryUI() {
    Utils::printHeader("INVOICE LOGS");

    std::vector<Invoice> invoices = getAllActiveInvoices();
    if (invoices.empty()) {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "  No invoices recorded in system.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    // Sort invoices in reverse chronological order (newest first)
    std::reverse(invoices.begin(), invoices.end());

    const size_t pageSize = 5;
    size_t totalPages = (invoices.size() + pageSize - 1) / pageSize;
    size_t currentPage = 0;

    while (true) {
        Utils::printHeader("INVOICE LOGS (Page " + std::to_string(currentPage + 1) + " of " + std::to_string(totalPages) + ")");

        Utils::setColor(Utils::Color::BOLD_WHITE);
        std::cout << "  "
                  << Utils::padString("Inv No", 8) << " | "
                  << Utils::padString("Date", 12) << " | "
                  << Utils::padString("Customer Name", 20) << " | "
                  << Utils::padString("Grand Total", 12) << " | "
                  << Utils::padString("Status", 10) << "\n";
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  --------------------------------------------------------------------------------\n";
        Utils::resetColor();

        size_t startIdx = currentPage * pageSize;
        size_t endIdx = std::min(startIdx + pageSize, invoices.size());

        for (size_t i = startIdx; i < endIdx; ++i) {
            std::cout << "  "
                      << Utils::padString(std::to_string(invoices[i].getInvoiceNumber()), 8) << " | "
                      << Utils::padString(invoices[i].getDate(), 12) << " | "
                      << Utils::padString(invoices[i].getCustomerName(), 20) << " | "
                      << Utils::padString(std::to_string(int(invoices[i].getGrandTotal())), 12) << " | "
                      << Utils::padString((invoices[i].getIsPaid() ? "Paid" : "Unpaid"), 10) << "\n";
        }

        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  --------------------------------------------------------------------------------\n";
        Utils::resetColor();

        std::cout << "  [N] Next Page  |  [P] Previous Page  |  [V] View Detailed Invoice  |  [Q] Exit Logs\n";
        std::string choice = Utils::getNonEmptyString("  Enter Choice: ");
        choice = Utils::toLowerCase(choice);

        if (choice == "n") {
            if (currentPage + 1 < totalPages) currentPage++;
        } else if (choice == "p") {
            if (currentPage > 0) currentPage--;
        } else if (choice == "v") {
            int invNum = Utils::getValidatedInt("  Enter Invoice Number to view details: ");
            Invoice details;
            if (findByInvoiceNumber(invNum, details)) {
                details.printInvoiceCard();
                Utils::pressEnterToContinue();
            } else {
                Utils::setColor(Utils::Color::RED);
                std::cout << "  Error: Invoice not found.\n";
                Utils::resetColor();
                Utils::pressEnterToContinue();
            }
        } else if (choice == "q") {
            break;
        }
    }
}
