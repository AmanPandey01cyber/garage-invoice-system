#include "Utils.h"
#include "Customer.h"
#include "Vehicle.h"
#include "Employee.h"
#include "Service.h"
#include "Part.h"
#include "Invoice.h"
#include "Payment.h"
#include "FileManager.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>

// Report operations forward declarations
void displayReportsMenu();
void runDailyRevenueReport();
void runMonthlyRevenueReport();
void runTopServicesReport();
void runMostUsedPartsReport();
void runCustomerHistoryReport();
void runVehicleHistoryReport();

// Administrative Dashboard & Main Navigation
void displayDashboard();
void customerManagementMenu();
void vehicleManagementMenu();
void employeeManagementMenu();
void serviceManagementMenu();
void partManagementMenu();
void billingManagementMenu();

bool adminLogin() {
    Utils::printHeader("ADMIN SECURE LOGIN");
    std::string username = Utils::getNonEmptyString("  Enter Username: ");
    std::string password = Utils::getMaskedPassword("  Enter Password: ");

    if (username == "admin" && password == "admin123") {
        Utils::setColor(Utils::Color::GREEN);
        std::cout << "\n  Access Granted! Redirecting to Dashboard...\n";
        Utils::resetColor();
        Utils::sleepMs(1000);
        return true;
    } else {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Access Denied! Incorrect username or password.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return false;
    }
}

int main() {
    // Initialize files and seed defaults
    FileManager::initializeDatabases();

    // Show loading screen
    Utils::loadingScreen();

    // Perform Admin Login
    while (!adminLogin()) {
        // Keep looping until user logs in or closes console
    }

    // Enter Main Menu Loop
    while (true) {
        displayDashboard();
        
        Utils::setColor(Utils::Color::BOLD_WHITE);
        std::cout << "  1. Customer Management\n";
        std::cout << "  2. Vehicle Management\n";
        std::cout << "  3. Employee Management\n";
        std::cout << "  4. Service Catalog Management\n";
        std::cout << "  5. Spare Parts Inventory\n";
        std::cout << "  6. Invoicing & Billing\n";
        std::cout << "  7. Analytical Reports\n";
        std::cout << "  8. Database Maintenance\n";
        std::cout << "  9. Logout & Exit\n\n";
        Utils::resetColor();

        int choice = Utils::getValidatedInt("  Select Menu Option (1-9): ", 1, 9);

        switch (choice) {
            case 1: customerManagementMenu(); break;
            case 2: vehicleManagementMenu(); break;
            case 3: employeeManagementMenu(); break;
            case 4: serviceManagementMenu(); break;
            case 5: partManagementMenu(); break;
            case 6: billingManagementMenu(); break;
            case 7: displayReportsMenu(); break;
            case 8: FileManager::displayMaintenanceDashboardUI(); break;
            case 9:
                Utils::printHeader("LOGOUT");
                std::cout << "  Thank you for using Garage Invoice Management System.\n  Goodbye!\n\n";
                Utils::sleepMs(1000);
                return 0;
        }
    }
    return 0;
}

void displayDashboard() {
    Utils::printHeader("ADMIN DASHBOARD");

    // Fetch Stats
    auto activeCusts = Customer::getAllActiveCustomers().size();
    auto activeVehs = Vehicle::getAllActiveVehicles().size();
    auto activeEmps = Employee::getAllActiveEmployees().size();
    auto activeServices = Service::getAllActiveServices().size();
    auto activeInvs = Invoice::getAllActiveInvoices();

    double revenueToday = 0.0;
    std::string today = Utils::getCurrentDate();
    for (const auto& inv : activeInvs) {
        if (inv.getDate() == today && inv.getIsPaid()) {
            revenueToday += inv.getGrandTotal();
        }
    }

    Utils::setColor(Utils::Color::YELLOW);
    std::cout << "  System Counters:\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  +----------------------+----------------------+----------------------+\n";
    std::cout << "  | Total Customers: " << Utils::padString(std::to_string(activeCusts), 3, ' ', true) << " | "
              << "Total Vehicles:  " << Utils::padString(std::to_string(activeVehs), 3, ' ', true) << " | "
              << "Total Employees: " << Utils::padString(std::to_string(activeEmps), 3, ' ', true) << " |\n";
    std::cout << "  | Total Services:  " << Utils::padString(std::to_string(activeServices), 3, ' ', true) << " | "
              << "Total Invoices:  " << Utils::padString(std::to_string(activeInvs.size()), 3, ' ', true) << " | "
              << "Today's Revenue: " << Utils::padString("Rs " + std::to_string(int(revenueToday)), 3, ' ', true) << " |\n";
    std::cout << "  +----------------------+----------------------+----------------------+\n";
    Utils::resetColor();
    std::cout << "\n";
}

void customerManagementMenu() {
    while (true) {
        Utils::printHeader("CUSTOMER MANAGEMENT");
        std::cout << "  1. Add Customer\n";
        std::cout << "  2. Display All Customers\n";
        std::cout << "  3. Search Customer\n";
        std::cout << "  4. Update Customer Details\n";
        std::cout << "  5. Delete Customer Record\n";
        std::cout << "  6. Back to Main Dashboard\n\n";

        int choice = Utils::getValidatedInt("  Enter Choice (1-6): ", 1, 6);
        if (choice == 6) break;

        switch (choice) {
            case 1: Customer::addCustomerUI(); break;
            case 2: Customer::displayAllCustomersUI(); break;
            case 3: Customer::searchCustomerUI(); break;
            case 4: Customer::updateCustomerUI(); break;
            case 5: Customer::deleteCustomerUI(); break;
        }
    }
}

void vehicleManagementMenu() {
    while (true) {
        Utils::printHeader("VEHICLE MANAGEMENT");
        std::cout << "  1. Register Vehicle\n";
        std::cout << "  2. Display All Registered Vehicles\n";
        std::cout << "  3. Search Vehicle Registry\n";
        std::cout << "  4. Update Vehicle Details\n";
        std::cout << "  5. Delete Vehicle Registration\n";
        std::cout << "  6. Back to Main Dashboard\n\n";

        int choice = Utils::getValidatedInt("  Enter Choice (1-6): ", 1, 6);
        if (choice == 6) break;

        switch (choice) {
            case 1: Vehicle::registerVehicleUI(); break;
            case 2: Vehicle::displayAllVehiclesUI(); break;
            case 3: Vehicle::searchVehicleUI(); break;
            case 4: Vehicle::updateVehicleUI(); break;
            case 5: Vehicle::deleteVehicleUI(); break;
        }
    }
}

void employeeManagementMenu() {
    while (true) {
        Utils::printHeader("EMPLOYEE MANAGEMENT");
        std::cout << "  1. Add Employee\n";
        std::cout << "  2. Display All Employees\n";
        std::cout << "  3. Search Employee Details\n";
        std::cout << "  4. Update Employee Salary/Details\n";
        std::cout << "  5. Delete Employee Record\n";
        std::cout << "  6. Back to Main Dashboard\n\n";

        int choice = Utils::getValidatedInt("  Enter Choice (1-6): ", 1, 6);
        if (choice == 6) break;

        switch (choice) {
            case 1: Employee::addEmployeeUI(); break;
            case 2: Employee::displayAllEmployeesUI(); break;
            case 3: Employee::searchEmployeeUI(); break;
            case 4: Employee::updateEmployeeUI(); break;
            case 5: Employee::deleteEmployeeUI(); break;
        }
    }
}

void serviceManagementMenu() {
    while (true) {
        Utils::printHeader("SERVICE CATALOG MANAGEMENT");
        std::cout << "  1. Add New Service Option\n";
        std::cout << "  2. Display Active Services\n";
        std::cout << "  3. Search Services\n";
        std::cout << "  4. Update Service Pricing/Details\n";
        std::cout << "  5. Delete Service Option\n";
        std::cout << "  6. Back to Main Dashboard\n\n";

        int choice = Utils::getValidatedInt("  Enter Choice (1-6): ", 1, 6);
        if (choice == 6) break;

        switch (choice) {
            case 1: Service::addServiceUI(); break;
            case 2: Service::displayAllServicesUI(); break;
            case 3: Service::searchServiceUI(); break;
            case 4: Service::updateServiceUI(); break;
            case 5: Service::deleteServiceUI(); break;
        }
    }
}

void partManagementMenu() {
    while (true) {
        Utils::printHeader("SPARE PARTS CATALOG MANAGEMENT");
        std::cout << "  1. Add New Spare Part\n";
        std::cout << "  2. Display Parts Inventory\n";
        std::cout << "  3. Search Parts Registry\n";
        std::cout << "  4. Update Part Stock/Pricing\n";
        std::cout << "  5. Delete Spare Part Record\n";
        std::cout << "  6. Back to Main Dashboard\n\n";

        int choice = Utils::getValidatedInt("  Enter Choice (1-6): ", 1, 6);
        if (choice == 6) break;

        switch (choice) {
            case 1: Part::addPartUI(); break;
            case 2: Part::displayAllPartsUI(); break;
            case 3: Part::searchPartUI(); break;
            case 4: Part::updatePartUI(); break;
            case 5: Part::deletePartUI(); break;
        }
    }
}

void billingManagementMenu() {
    while (true) {
        Utils::printHeader("INVOICING & BILLING");
        std::cout << "  1. Generate Customer Invoice\n";
        std::cout << "  2. Search Historical Invoices\n";
        std::cout << "  3. Display Invoice Logs\n";
        std::cout << "  4. Back to Main Dashboard\n\n";

        int choice = Utils::getValidatedInt("  Enter Choice (1-4): ", 1, 4);
        if (choice == 4) break;

        switch (choice) {
            case 1: Invoice::createInvoiceUI(); break;
            case 2: Invoice::searchInvoiceUI(); break;
            case 3: Invoice::viewInvoiceHistoryUI(); break;
        }
    }
}

void displayReportsMenu() {
    while (true) {
        Utils::printHeader("ANALYTICAL REPORTS");
        std::cout << "  1. Daily Revenue Report\n";
        std::cout << "  2. Monthly Revenue Report\n";
        std::cout << "  3. Top Performing Services\n";
        std::cout << "  4. Most Utilized Spare Parts\n";
        std::cout << "  5. Customer History Profile\n";
        std::cout << "  6. Vehicle History Profile\n";
        std::cout << "  7. Return to Dashboard\n\n";

        int choice = Utils::getValidatedInt("  Enter Choice (1-7): ", 1, 7);
        if (choice == 7) break;

        switch (choice) {
            case 1: runDailyRevenueReport(); break;
            case 2: runMonthlyRevenueReport(); break;
            case 3: runTopServicesReport(); break;
            case 4: runMostUsedPartsReport(); break;
            case 5: runCustomerHistoryReport(); break;
            case 6: runVehicleHistoryReport(); break;
        }
    }
}

void runDailyRevenueReport() {
    Utils::printHeader("DAILY REVENUE REPORT");
    std::cout << "  Enter date (YYYY-MM-DD) or press Enter for today (" << Utils::getCurrentDate() << "): ";
    std::string date;
    std::getline(std::cin, date);
    date = Utils::trim(date);
    if (date.empty()) date = Utils::getCurrentDate();

    while (!Utils::isValidDate(date)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "  Error: Invalid format. Enter YYYY-MM-DD: ";
        Utils::resetColor();
        std::getline(std::cin, date);
        date = Utils::trim(date);
    }

    std::vector<Invoice> invoices = Invoice::getAllActiveInvoices();
    double totalEarned = 0.0;
    int transactionCount = 0;

    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "\n  Paid Invoices on " << date << ":\n";
    std::cout << "  "
              << Utils::padString("Inv No", 8) << " | "
              << Utils::padString("Customer Name", 25) << " | "
              << Utils::padString("Grand Total", 15) << " | "
              << Utils::padString("Payment Mode", 15) << "\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  ----------------------------------------------------------------------------\n";
    Utils::resetColor();

    for (const auto& inv : invoices) {
        if (inv.getDate() == date && inv.getIsPaid()) {
            std::cout << "  "
                      << Utils::padString(std::to_string(inv.getInvoiceNumber()), 8) << " | "
                      << Utils::padString(inv.getCustomerName(), 25) << " | "
                      << Utils::padString("Rs " + std::to_string(int(inv.getGrandTotal())), 15) << " | "
                      << Utils::padString(Payment::modeToString(inv.getPaymentMode()), 15) << "\n";
            totalEarned += inv.getGrandTotal();
            transactionCount++;
        }
    }

    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  ----------------------------------------------------------------------------\n";
    Utils::resetColor();
    Utils::setColor(Utils::Color::GREEN);
    std::cout << "  Total Transactions: " << transactionCount << "\n";
    std::cout << "  Total Net Revenue : Rs " << std::fixed << std::setprecision(2) << totalEarned << "\n";
    Utils::resetColor();
    Utils::pressEnterToContinue();
}

void runMonthlyRevenueReport() {
    Utils::printHeader("MONTHLY REVENUE REPORT");
    std::string yearStr = std::to_string(Utils::getValidatedInt("  Enter Year (1990-2100): ", 1990, 2100));
    int monthVal = Utils::getValidatedInt("  Enter Month (1-12): ", 1, 12);
    
    std::stringstream ss;
    ss << yearStr << "-" << std::setw(2) << std::setfill('0') << monthVal;
    std::string targetPrefix = ss.str(); // e.g. "2026-07"

    std::vector<Invoice> invoices = Invoice::getAllActiveInvoices();
    double totalEarned = 0.0;
    int transactionCount = 0;

    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "\n  Paid Invoices in Month " << targetPrefix << ":\n";
    std::cout << "  "
              << Utils::padString("Inv No", 8) << " | "
              << Utils::padString("Date", 12) << " | "
              << Utils::padString("Customer Name", 25) << " | "
              << Utils::padString("Grand Total", 15) << "\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  ----------------------------------------------------------------------------\n";
    Utils::resetColor();

    for (const auto& inv : invoices) {
        if (inv.getDate().find(targetPrefix) == 0 && inv.getIsPaid()) {
            std::cout << "  "
                      << Utils::padString(std::to_string(inv.getInvoiceNumber()), 8) << " | "
                      << Utils::padString(inv.getDate(), 12) << " | "
                      << Utils::padString(inv.getCustomerName(), 25) << " | "
                      << Utils::padString("Rs " + std::to_string(int(inv.getGrandTotal())), 15) << "\n";
            totalEarned += inv.getGrandTotal();
            transactionCount++;
        }
    }

    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  ----------------------------------------------------------------------------\n";
    Utils::resetColor();
    Utils::setColor(Utils::Color::GREEN);
    std::cout << "  Total Transactions: " << transactionCount << "\n";
    std::cout << "  Total Net Revenue : Rs " << std::fixed << std::setprecision(2) << totalEarned << "\n";
    Utils::resetColor();
    Utils::pressEnterToContinue();
}

void runTopServicesReport() {
    Utils::printHeader("TOP PERFORMING SERVICES");
    std::vector<Invoice> invoices = Invoice::getAllActiveInvoices();
    
    std::map<int, int> serviceUsage;
    for (const auto& inv : invoices) {
        for (int sId : inv.getServiceIds()) {
            serviceUsage[sId]++;
        }
    }

    if (serviceUsage.empty()) {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "  No service logs recorded in system invoices.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    std::vector<std::pair<int, int>> usageVec(serviceUsage.begin(), serviceUsage.end());
    std::sort(usageVec.begin(), usageVec.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; // Descending
    });

    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "  Rank | Service ID | " << Utils::padString("Service Name", 30) << " | Jobs Done\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  ------------------------------------------------------------------\n";
    Utils::resetColor();

    int rank = 1;
    for (const auto& item : usageVec) {
        Service srv;
        std::string sName = "Unknown Service";
        if (Service::findById(item.first, srv)) {
            sName = srv.getName();
        }
        std::cout << "   " << std::setw(3) << rank++ << " | "
                  << std::setw(10) << item.first << " | "
                  << Utils::padString(sName, 30) << " | "
                  << std::setw(9) << item.second << "\n";
    }
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  ------------------------------------------------------------------\n";
    Utils::resetColor();
    Utils::pressEnterToContinue();
}

void runMostUsedPartsReport() {
    Utils::printHeader("MOST UTILIZED SPARE PARTS");
    std::vector<Invoice> invoices = Invoice::getAllActiveInvoices();

    std::map<int, int> partUsage;
    for (const auto& inv : invoices) {
        for (const auto& item : inv.getPartsList()) {
            partUsage[item.first] += item.second;
        }
    }

    if (partUsage.empty()) {
        Utils::setColor(Utils::Color::YELLOW);
        std::cout << "  No spare parts sales recorded in system invoices.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    std::vector<std::pair<int, int>> usageVec(partUsage.begin(), partUsage.end());
    std::sort(usageVec.begin(), usageVec.end(), [](const auto& a, const auto& b) {
        return a.second > b.second; // Descending
    });

    Utils::setColor(Utils::Color::BOLD_WHITE);
    std::cout << "  Rank | Part ID    | " << Utils::padString("Part Name", 30) << " | Qty Sold\n";
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  ------------------------------------------------------------------\n";
    Utils::resetColor();

    int rank = 1;
    for (const auto& item : usageVec) {
        Part part;
        std::string pName = "Unknown Part";
        if (Part::findById(item.first, part)) {
            pName = part.getName();
        }
        std::cout << "   " << std::setw(3) << rank++ << " | "
                  << std::setw(10) << item.first << " | "
                  << Utils::padString(pName, 30) << " | "
                  << std::setw(8) << item.second << "\n";
    }
    Utils::setColor(Utils::Color::CYAN);
    std::cout << "  ------------------------------------------------------------------\n";
    Utils::resetColor();
    Utils::pressEnterToContinue();
}

void runCustomerHistoryReport() {
    Utils::printHeader("CUSTOMER PROFILE HISTORY");
    int targetId = Utils::getValidatedInt("  Enter Customer ID: ");

    Customer cust;
    if (!Customer::findById(targetId, cust)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Customer ID " << targetId << " not found.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    std::cout << "\n";
    cust.displayCard();

    // Vehicles
    std::vector<Vehicle> vehicles = Vehicle::findByCustomerId(targetId);
    std::cout << "\n  Registered Vehicles:\n";
    if (vehicles.empty()) {
        std::cout << "    (No vehicles registered under this owner)\n";
    } else {
        for (const auto& v : vehicles) {
            std::cout << "    - Plate No: " << Utils::padString(v.getVehicleNumber(), 12) 
                      << " | Model: " << v.getBrand() << " " << v.getModel() << "\n";
        }
    }

    // Historical billing logs
    std::vector<Invoice> list = Invoice::findByCustomerId(targetId);
    std::cout << "\n  Historical Invoices:\n";
    if (list.empty()) {
        std::cout << "    (No historical transactions found)\n";
    } else {
        Utils::setColor(Utils::Color::BOLD_WHITE);
        std::cout << "    "
                  << Utils::padString("Inv No", 8) << " | "
                  << Utils::padString("Date", 12) << " | "
                  << Utils::padString("Grand Total", 15) << " | "
                  << Utils::padString("Status", 10) << "\n";
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "    ----------------------------------------------------------\n";
        Utils::resetColor();

        double totalSpent = 0.0;
        int transactionCount = 0;

        for (const auto& inv : list) {
            std::cout << "    "
                      << Utils::padString(std::to_string(inv.getInvoiceNumber()), 8) << " | "
                      << Utils::padString(inv.getDate(), 12) << " | "
                      << Utils::padString("Rs " + std::to_string(int(inv.getGrandTotal())), 15) << " | "
                      << Utils::padString((inv.getIsPaid() ? "Paid" : "Unpaid"), 10) << "\n";
            
            if (inv.getIsPaid()) {
                totalSpent += inv.getGrandTotal();
                transactionCount++;
            }
        }
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "    ----------------------------------------------------------\n";
        Utils::resetColor();
        Utils::setColor(Utils::Color::GREEN);
        std::cout << "    Total Spent (Paid Bills): Rs " << std::fixed << std::setprecision(2) << totalSpent << "\n";
        Utils::resetColor();
    }
    Utils::pressEnterToContinue();
}

void runVehicleHistoryReport() {
    Utils::printHeader("VEHICLE REPAIR HISTORY");
    std::string rawVehNo = Utils::getNonEmptyString("  Enter Vehicle Number: ");
    std::string vehNo = Utils::trim(rawVehNo);
    std::transform(vehNo.begin(), vehNo.end(), vehNo.begin(), ::toupper);

    Vehicle veh;
    if (!Vehicle::findByNumber(vehNo, veh)) {
        Utils::setColor(Utils::Color::RED);
        std::cout << "\n  Vehicle number " << vehNo << " not found in system.\n";
        Utils::resetColor();
        Utils::pressEnterToContinue();
        return;
    }

    std::cout << "\n";
    veh.displayCard();

    // History of invoices
    std::vector<Invoice> list = Invoice::findByVehicleNumber(vehNo);
    std::cout << "\n  Repair Logs & Jobs history:\n";
    if (list.empty()) {
        std::cout << "    (No service/billing logs found for this vehicle)\n";
    } else {
        Utils::setColor(Utils::Color::BOLD_WHITE);
        std::cout << "    "
                  << Utils::padString("Inv No", 8) << " | "
                  << Utils::padString("Date", 12) << " | "
                  << Utils::padString("Job Summary (Services Performing)", 35) << "\n";
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "    --------------------------------------------------------------------------\n";
        Utils::resetColor();

        for (const auto& inv : list) {
            std::string jobSummary = "";
            for (size_t i = 0; i < inv.getServiceIds().size(); ++i) {
                Service s;
                if (Service::findById(inv.getServiceIds()[i], s)) {
                    jobSummary += s.getName();
                    if (i + 1 < inv.getServiceIds().size()) jobSummary += ", ";
                }
            }
            if (jobSummary.length() > 33) {
                jobSummary = jobSummary.substr(0, 30) + "...";
            }

            std::cout << "    "
                      << Utils::padString(std::to_string(inv.getInvoiceNumber()), 8) << " | "
                      << Utils::padString(inv.getDate(), 12) << " | "
                      << Utils::padString(jobSummary, 35) << "\n";
        }
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "    --------------------------------------------------------------------------\n";
        Utils::resetColor();
    }
    Utils::pressEnterToContinue();
}
