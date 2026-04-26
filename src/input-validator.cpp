#include "input-validator.h"
#include <iostream>
#include <limits>

std::string InputValidator::getValidatedEmail(const std::string& prompt, int maxRetries) {
    std::string input;
    for (int i = 0; i < maxRetries; ++i) {
        std::cout << prompt;
        std::getline(std::cin, input);
        input = input; // Trim would go here if we had access to it
        
        if (ValidationUtils::isValidEmail(input)) {
            return input;
        }
        
        if (i < maxRetries - 1) {
            printError("Invalid email format. Please try again.");
        }
    }
    printError("Failed to get valid email after " + std::to_string(maxRetries) + " attempts.");
    return "";
}

std::string InputValidator::getValidatedPhone(const std::string& prompt, int maxRetries) {
    std::string input;
    for (int i = 0; i < maxRetries; ++i) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (ValidationUtils::isValidPhoneNumber(input)) {
            return input;
        }
        
        if (i < maxRetries - 1) {
            printError("Invalid phone number format. Expected 10-15 digits.");
        }
    }
    printError("Failed to get valid phone number after " + std::to_string(maxRetries) + " attempts.");
    return "";
}

std::string InputValidator::getValidatedDate(const std::string& prompt, int maxRetries) {
    std::string input;
    for (int i = 0; i < maxRetries; ++i) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (ValidationUtils::isValidDate(input)) {
            return input;
        }
        
        if (i < maxRetries - 1) {
            printError("Invalid date format. Please use DD-MM-YYYY format.");
        }
    }
    printError("Failed to get valid date after " + std::to_string(maxRetries) + " attempts.");
    return "";
}

std::string InputValidator::getValidatedCNIC(const std::string& prompt, int maxRetries) {
    std::string input;
    for (int i = 0; i < maxRetries; ++i) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (ValidationUtils::isValidCNIC(input)) {
            return input;
        }
        
        if (i < maxRetries - 1) {
            printError("Invalid CNIC format. Expected XXXXX-XXXXXXX-X format.");
        }
    }
    printError("Failed to get valid CNIC after " + std::to_string(maxRetries) + " attempts.");
    return "";
}

std::string InputValidator::getValidatedMonthYear(const std::string& prompt, int maxRetries) {
    std::string input;
    for (int i = 0; i < maxRetries; ++i) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (ValidationUtils::isValidMonthYear(input)) {
            return input;
        }
        
        if (i < maxRetries - 1) {
            printError("Invalid format. Please use 'Month YYYY' format (e.g., January 2025).");
        }
    }
    printError("Failed to get valid month-year after " + std::to_string(maxRetries) + " attempts.");
    return "";
}

int InputValidator::getValidatedAge(const std::string& prompt, int maxRetries) {
    std::string input;
    for (int i = 0; i < maxRetries; ++i) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (ValidationUtils::isDigitOnly(input)) {
            int age = std::stoi(input);
            if (ValidationUtils::isValidAge(age)) {
                return age;
            }
        }
        
        if (i < maxRetries - 1) {
            printError("Invalid age. Please enter a number between 18 and 120.");
        }
    }
    printError("Failed to get valid age after " + std::to_string(maxRetries) + " attempts.");
    return -1;
}

double InputValidator::getValidatedIncome(const std::string& prompt, int maxRetries) {
    std::string input;
    for (int i = 0; i < maxRetries; ++i) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (ValidationUtils::isNumeric(input)) {
            double income = std::stod(input);
            if (ValidationUtils::isValidIncome(income)) {
                return income;
            }
        }
        
        if (i < maxRetries - 1) {
            printError("Invalid income. Please enter a positive number.");
        }
    }
    printError("Failed to get valid income after " + std::to_string(maxRetries) + " attempts.");
    return -1.0;
}

double InputValidator::getValidatedPayment(const std::string& prompt, int maxRetries) {
    std::string input;
    for (int i = 0; i < maxRetries; ++i) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (ValidationUtils::isNumeric(input)) {
            double payment = std::stod(input);
            if (ValidationUtils::isValidMonthlyPayment(payment)) {
                return payment;
            }
        }
        
        if (i < maxRetries - 1) {
            printError("Invalid payment amount. Please enter a positive number.");
        }
    }
    printError("Failed to get valid payment after " + std::to_string(maxRetries) + " attempts.");
    return -1.0;
}

std::string InputValidator::getValidatedInput(const std::string& prompt, bool canBeEmpty, int maxRetries) {
    std::string input;
    for (int i = 0; i < maxRetries; ++i) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        if (canBeEmpty || !ValidationUtils::isEmpty(input)) {
            return input;
        }
        
        if (i < maxRetries - 1) {
            printError("Input cannot be empty. Please try again.");
        }
    }
    printError("Failed to get valid input after " + std::to_string(maxRetries) + " attempts.");
    return "";
}

bool InputValidator::validateEmailSilent(const std::string& email) {
    return ValidationUtils::isValidEmail(email);
}

bool InputValidator::validatePhoneSilent(const std::string& phone) {
    return ValidationUtils::isValidPhoneNumber(phone);
}

bool InputValidator::validateDateSilent(const std::string& date) {
    return ValidationUtils::isValidDate(date);
}

bool InputValidator::validateCNICSilent(const std::string& cnic) {
    return ValidationUtils::isValidCNIC(cnic);
}

void InputValidator::printError(const std::string& errorMessage) {
    std::cout << "\033[1;31m[ERROR]\033[0m " << errorMessage << std::endl;
}

void InputValidator::printInfo(const std::string& message) {
    std::cout << "\033[1;32m[INFO]\033[0m " << message << std::endl;
}
