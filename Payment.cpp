#include "Payment.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace Payment {

    std::string modeToString(Mode mode) {
        switch (mode) {
            case Mode::CASH: return "Cash";
            case Mode::UPI: return "UPI";
            case Mode::CREDIT_CARD: return "Credit Card";
            case Mode::DEBIT_CARD: return "Debit Card";
            case Mode::NET_BANKING: return "Net Banking";
            default: return "Unknown";
        }
    }

    Mode getPaymentModeUI() {
        std::cout << "\n  Select Payment Mode:\n";
        std::cout << "  1. Cash\n";
        std::cout << "  2. UPI (GPay, PhonePe, Paytm)\n";
        std::cout << "  3. Credit Card\n";
        std::cout << "  4. Debit Card\n";
        std::cout << "  5. Net Banking\n\n";
        
        int choice = Utils::getValidatedInt("  Enter choice (1-5): ", 1, 5);
        return static_cast<Mode>(choice);
    }

    bool processPayment(double amount, Mode mode, std::string& refNumber) {
        Utils::printHeader("PROCESS PAYMENT");
        std::cout << "  Total Payable Amount: Rs " << std::fixed << std::setprecision(2) << amount << "\n";
        std::cout << "  Selected Mode: " << modeToString(mode) << "\n\n";

        refNumber = "N/A";

        if (mode == Mode::CASH) {
            double cashReceived = Utils::getValidatedDouble("  Enter Cash Received (Rs): ", amount, 10000000.0);
            double change = cashReceived - amount;
            if (change > 0) {
                Utils::setColor(Utils::Color::GREEN);
                std::cout << "\n  Change to Return: Rs " << std::fixed << std::setprecision(2) << change << "\n";
                Utils::resetColor();
            }
            refNumber = "CASH_TXN";
        } 
        else if (mode == Mode::UPI) {
            std::cout << "  Scan QR code or request UPI link for Rs " << amount << "\n";
            std::string vpa = Utils::getNonEmptyString("  Enter Customer UPI ID / Phone (e.g., name@okbank): ");
            std::string txnId = Utils::getNonEmptyString("  Enter 12-digit UPI Ref/Transaction ID: ");
            while (txnId.length() < 6 || txnId.length() > 16) {
                Utils::setColor(Utils::Color::RED);
                std::cout << "  Error: Enter a valid transaction ID.\n";
                Utils::resetColor();
                txnId = Utils::getNonEmptyString("  Enter UPI Ref/Transaction ID: ");
            }
            refNumber = "UPI:" + txnId + " (" + vpa + ")";
        } 
        else if (mode == Mode::CREDIT_CARD || mode == Mode::DEBIT_CARD) {
            std::cout << "  Insert/Swipe Card on POS Terminal.\n";
            std::string last4 = Utils::getNonEmptyString("  Enter Last 4 digits of Card: ");
            while (last4.length() != 4 || !std::all_of(last4.begin(), last4.end(), ::isdigit)) {
                Utils::setColor(Utils::Color::RED);
                std::cout << "  Error: Must be exactly 4 digits.\n";
                Utils::resetColor();
                last4 = Utils::getNonEmptyString("  Enter Last 4 digits of Card: ");
            }
            std::string authCode = Utils::getNonEmptyString("  Enter POS Approval/Auth Code: ");
            refNumber = (mode == Mode::CREDIT_CARD ? "CC:" : "DC:") + last4 + "-AUTH:" + authCode;
        } 
        else if (mode == Mode::NET_BANKING) {
            std::string bankName = Utils::getNonEmptyString("  Enter Bank Name: ");
            std::string bankRef = Utils::getNonEmptyString("  Enter Internet Banking Ref ID: ");
            refNumber = bankName + "-REF:" + bankRef;
        }

        Utils::showProgress("Authorizing Transaction", 1000);
        Utils::setColor(Utils::Color::GREEN);
        std::cout << "  Payment of Rs " << amount << " approved successfully!\n";
        Utils::resetColor();
        return true;
    }

    void generateReceipt(int invoiceNumber, double amount, Mode mode, const std::string& refNumber, const std::string& date) {
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "\n  --------------------------------------------------------\n";
        std::cout << "                      PAYMENT RECEIPT                     \n";
        std::cout << "  --------------------------------------------------------\n";
        Utils::resetColor();
        std::cout << "  Invoice No     : " << invoiceNumber << "\n";
        std::cout << "  Date           : " << date << "\n";
        std::cout << "  Payment Mode   : " << modeToString(mode) << "\n";
        std::cout << "  Reference No   : " << refNumber << "\n";
        std::cout << "  Amount Paid    : Rs " << std::fixed << std::setprecision(2) << amount << "\n";
        Utils::setColor(Utils::Color::GREEN);
        std::cout << "  Transaction    : SUCCESSFUL\n";
        Utils::setColor(Utils::Color::CYAN);
        std::cout << "  --------------------------------------------------------\n";
        Utils::resetColor();
    }
}
