#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>

namespace Payment {
    enum class Mode {
        CASH = 1,
        UPI,
        CREDIT_CARD,
        DEBIT_CARD,
        NET_BANKING
    };

    std::string modeToString(Mode mode);
    Mode getPaymentModeUI();
    
    // Processes checkout and retrieves details
    bool processPayment(double amount, Mode mode, std::string& refNumber);
    
    // Receipt Generator
    void generateReceipt(int invoiceNumber, double amount, Mode mode, const std::string& refNumber, const std::string& date);
}

#endif // PAYMENT_H
