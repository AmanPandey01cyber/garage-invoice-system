#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {
    // Console formatting
    enum class Color {
        RESET = 0,
        RED,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        BOLD_WHITE
    };

    void setColor(Color color);
    void resetColor();
    void clearScreen();
    void printHeader(const std::string& title);
    void loadingScreen();
    void showProgress(const std::string& message, int durationMs = 1000);
    void pressEnterToContinue();
    void sleepMs(int durationMs);

    // Password Masking
    std::string getMaskedPassword(const std::string& prompt);

    // Input Validation Helpers
    bool isValidPhone(const std::string& phone);
    bool isValidEmail(const std::string& email);
    bool isValidDate(const std::string& date); // Format: YYYY-MM-DD
    bool isValidYear(int year);
    bool isValidName(const std::string& name);

    // Safe Input Retrievers
    std::string getNonEmptyString(const std::string& prompt);
    std::string getValidatedPhone(const std::string& prompt);
    std::string getValidatedEmail(const std::string& prompt);
    int getValidatedInt(const std::string& prompt, int minVal = 0, int maxVal = 2000000000);
    double getValidatedDouble(const std::string& prompt, double minVal = 0.0, double maxVal = 1000000000.0);
    bool getConfirmation(const std::string& prompt);

    // String Manipulation
    std::string trim(const std::string& str);
    std::string padString(const std::string& str, size_t width, char paddingChar = ' ', bool rightAlign = false);
    std::string toLowerCase(const std::string& str);

    // Date Time
    std::string getCurrentDate();
}

#endif // UTILS_H
