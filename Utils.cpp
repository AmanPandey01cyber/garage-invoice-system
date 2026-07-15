#include "Utils.h"
#include <iostream>
#include <regex>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

namespace Utils {

#ifdef _WIN32
    void setColor(Color color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD wAttributes = 0;
        switch (color) {
            case Color::RESET:
                wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                break;
            case Color::RED:
                wAttributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
                break;
            case Color::GREEN:
                wAttributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                break;
            case Color::YELLOW:
                wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
                break;
            case Color::BLUE:
                wAttributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                break;
            case Color::MAGENTA:
                wAttributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                break;
            case Color::CYAN:
                wAttributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                break;
            case Color::WHITE:
                wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
                break;
            case Color::BOLD_WHITE:
                wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
                break;
        }
        SetConsoleTextAttribute(hConsole, wAttributes);
    }
#else
    void setColor(Color color) {
        switch (color) {
            case Color::RESET:
                std::cout << "\033[0m";
                break;
            case Color::RED:
                std::cout << "\033[1;31m";
                break;
            case Color::GREEN:
                std::cout << "\033[1;32m";
                break;
            case Color::YELLOW:
                std::cout << "\033[1;33m";
                break;
            case Color::BLUE:
                std::cout << "\033[1;34m";
                break;
            case Color::MAGENTA:
                std::cout << "\033[1;35m";
                break;
            case Color::CYAN:
                std::cout << "\033[1;36m";
                break;
            case Color::WHITE:
                std::cout << "\033[0;37m";
                break;
            case Color::BOLD_WHITE:
                std::cout << "\033[1;37m";
                break;
        }
    }
#endif

    void resetColor() {
        setColor(Color::RESET);
    }

    void clearScreen() {
#ifdef _WIN32
        std::system("cls");
#else
        std::system("clear");
#endif
    }

    void printHeader(const std::string& title) {
        clearScreen();
        setColor(Color::CYAN);
        std::cout << "================================================================================\n";
        std::cout << "                      " << padString(title, 36, ' ', false) << "                      \n";
        std::cout << "================================================================================\n";
        resetColor();
        std::cout << "\n";
    }

    void loadingScreen() {
        clearScreen();
        setColor(Color::BLUE);
        std::cout << R"(
    ===================================================================
    *                                                                 *
    *      GGGG    AAA   RRRR    AAA    GGGG  EEEEE                   *
    *     G       A   A  R   R  A   A  G      E                       *
    *     G  GG   AAAAA  RRRR   AAAAA  G  GG  EEEE                    *
    *     G   G   A   A  R  R   A   A  G   G  E                       *
    *      GGGG   A   A  R   R  A   A   GGGG  EEEEE                   *
    *                                                                 *
    *          IIIII N   N V   V  OOO  IIIII  CCCC EEEEE              *
    *            I   NN  N V   V O   O   I   C     E                  *
    *            I   N N N  V V  O   O   I   C     EEEE               *
    *            I   N  NN  V V  O   O   I   C     E                  *
    *          IIIII N   N   V    OOO  IIIII  CCCC EEEEE              *
    *                                                                 *
    ===================================================================
        )" << "\n";
        resetColor();
        showProgress("Initializing System Components", 1200);
    }

    void showProgress(const std::string& message, int durationMs) {
        setColor(Color::YELLOW);
        std::cout << "  " << message << "...\n  [";
        int barWidth = 40;
        int steps = 20;
        int sleepTime = durationMs / steps;
        for (int i = 0; i <= steps; ++i) {
            std::cout << "\r  " << message << "...\n  [";
            int pos = (barWidth * i) / steps;
            for (int j = 0; j < barWidth; ++j) {
                if (j < pos) std::cout << "=";
                else if (j == pos) std::cout << ">";
                else std::cout << " ";
            }
            std::cout << "] " << int((float)i / steps * 100.0) << "%";
            std::cout.flush();
            sleepMs(sleepTime);
            // Move cursor back up one line to redraw progress bar properly
            if (i < steps) {
                std::cout << "\033[A"; // ANSI code to move cursor up, works on modern Windows as well
            }
        }
        std::cout << "\n\n";
        resetColor();
    }

    void pressEnterToContinue() {
        setColor(Color::YELLOW);
        std::cout << "\n  Press Enter to continue...";
        resetColor();
        std::string temp;
        std::getline(std::cin, temp);
    }

    void sleepMs(int durationMs) {
#ifdef _WIN32
        Sleep(durationMs);
#else
        usleep(durationMs * 1000);
#endif
    }

    std::string getMaskedPassword(const std::string& prompt) {
        std::string password = "";
        std::cout << prompt;
#ifdef _WIN32
        char ch;
        while ((ch = _getch()) != '\r') { // carriage return is Enter key
            if (ch == '\b') { // backspace
                if (!password.empty()) {
                    password.pop_back();
                    std::cout << "\b \b";
                }
            } else if (ch == 27) { // Escape key
                // Ignore or handle
            } else if (ch == 0 || ch == -32) { // Function or arrow keys
                _getch(); // consume second byte
            } else {
                password.push_back(ch);
                std::cout << '*';
            }
        }
        std::cout << std::endl;
#else
        // POSIX terminal masking
        termios oldt;
        tcgetattr(STDIN_FILENO, &oldt);
        termios newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        std::getline(std::cin, password);

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        std::cout << std::endl;
#endif
        return password;
    }

    bool isValidPhone(const std::string& phone) {
        if (phone.length() < 10 || phone.length() > 12) return false;
        for (char c : phone) {
            if (!std::isdigit(c)) return false;
        }
        return true;
    }

    bool isValidEmail(const std::string& email) {
        const std::regex pattern(R"(^[a-zA-Z0-9_.+-]+@[a-zA-Z0-9-]+\.[a-zA-Z0-9-.]+$)");
        return std::regex_match(email, pattern);
    }

    bool isValidDate(const std::string& date) {
        // Date format: YYYY-MM-DD
        const std::regex pattern(R"(^\d{4}-\d{2}-\d{2}$)");
        if (!std::regex_match(date, pattern)) return false;

        int year, month, day;
        char dash1, dash2;
        std::stringstream ss(date);
        ss >> year >> dash1 >> month >> dash2 >> day;

        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;

        // Days in months
        if (month == 2) {
            bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (isLeap && day > 29) return false;
            if (!isLeap && day > 28) return false;
        } else if (month == 4 || month == 6 || month == 9 || month == 11) {
            if (day > 30) return false;
        }
        return true;
    }

    bool isValidYear(int year) {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        int currentYear = now->tm_year + 1900;
        return year >= 1900 && year <= currentYear + 1;
    }

    bool isValidName(const std::string& name) {
        if (name.empty()) return false;
        for (char c : name) {
            if (!std::isalpha(c) && !std::isspace(c) && c != '.') return false;
        }
        return true;
    }

    std::string getNonEmptyString(const std::string& prompt) {
        std::string value;
        while (true) {
            std::cout << prompt;
            if (std::getline(std::cin, value)) {
                value = trim(value);
                if (!value.empty()) {
                    return value;
                }
            }
            setColor(Color::RED);
            std::cout << "  Error: Input cannot be empty. Please try again.\n";
            resetColor();
        }
    }

    std::string getValidatedPhone(const std::string& prompt) {
        std::string value;
        while (true) {
            std::cout << prompt;
            if (std::getline(std::cin, value)) {
                value = trim(value);
                if (isValidPhone(value)) {
                    return value;
                }
            }
            setColor(Color::RED);
            std::cout << "  Error: Invalid phone number. Must be 10-12 digits.\n";
            resetColor();
        }
    }

    std::string getValidatedEmail(const std::string& prompt) {
        std::string value;
        while (true) {
            std::cout << prompt;
            if (std::getline(std::cin, value)) {
                value = trim(value);
                if (isValidEmail(value)) {
                    return value;
                }
            }
            setColor(Color::RED);
            std::cout << "  Error: Invalid email format (example@domain.com).\n";
            resetColor();
        }
    }

    int getValidatedInt(const std::string& prompt, int minVal, int maxVal) {
        std::string input;
        int value;
        while (true) {
            std::cout << prompt;
            if (std::getline(std::cin, input)) {
                std::stringstream ss(input);
                if (ss >> value) {
                    char extra;
                    if (!(ss >> extra) && value >= minVal && value <= maxVal) {
                        return value;
                    }
                }
            }
            setColor(Color::RED);
            std::cout << "  Error: Invalid input. Enter an integer between " << minVal << " and " << maxVal << ".\n";
            resetColor();
        }
    }

    double getValidatedDouble(const std::string& prompt, double minVal, double maxVal) {
        std::string input;
        double value;
        while (true) {
            std::cout << prompt;
            if (std::getline(std::cin, input)) {
                std::stringstream ss(input);
                if (ss >> value) {
                    char extra;
                    if (!(ss >> extra) && value >= minVal && value <= maxVal) {
                        return value;
                    }
                }
            }
            setColor(Color::RED);
            std::cout << "  Error: Invalid input. Enter a decimal between " << minVal << " and " << maxVal << ".\n";
            resetColor();
        }
    }

    bool getConfirmation(const std::string& prompt) {
        std::string input;
        while (true) {
            std::cout << prompt << " (y/n): ";
            if (std::getline(std::cin, input)) {
                input = trim(toLowerCase(input));
                if (input == "y" || input == "yes") return true;
                if (input == "n" || input == "no") return false;
            }
            setColor(Color::RED);
            std::cout << "  Error: Please enter 'y' or 'n'.\n";
            resetColor();
        }
    }

    std::string trim(const std::string& str) {
        if (str.empty()) return str;
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, (last - first + 1));
    }

    std::string padString(const std::string& str, size_t width, char paddingChar, bool rightAlign) {
        if (str.length() >= width) {
            return str.substr(0, width);
        }
        size_t paddingLength = width - str.length();
        std::string padding(paddingLength, paddingChar);
        if (rightAlign) {
            return padding + str;
        } else {
            return str + padding;
        }
    }

    std::string toLowerCase(const std::string& str) {
        std::string lower = str;
        std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        return lower;
    }

    std::string getCurrentDate() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_time);
        std::stringstream ss;
        ss << std::setfill('0')
           << std::setw(4) << (now_tm->tm_year + 1900) << "-"
           << std::setw(2) << (now_tm->tm_mon + 1) << "-"
           << std::setw(2) << now_tm->tm_mday;
        return ss.str();
    }
}
