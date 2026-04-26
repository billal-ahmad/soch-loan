#pragma once
#include <string>
#include "validation-utils.h"

/**
 * InputValidator - Wrapper around ValidationUtils for interactive input validation
 * 
 * Provides user-friendly validation with error messages and retry prompts
 */
class InputValidator {
public:
    // Get validated input from user with retries
    static std::string getValidatedEmail(const std::string& prompt = "Enter email: ", int maxRetries = 3);
    static std::string getValidatedPhone(const std::string& prompt = "Enter phone number: ", int maxRetries = 3);
    static std::string getValidatedDate(const std::string& prompt = "Enter date (DD-MM-YYYY): ", int maxRetries = 3);
    static std::string getValidatedCNIC(const std::string& prompt = "Enter CNIC (XXXXX-XXXXXXX-X): ", int maxRetries = 3);
    static std::string getValidatedMonthYear(const std::string& prompt = "Enter month year (e.g., January 2025): ", int maxRetries = 3);
    static int getValidatedAge(const std::string& prompt = "Enter age: ", int maxRetries = 3);
    static double getValidatedIncome(const std::string& prompt = "Enter annual income (PKR): ", int maxRetries = 3);
    static double getValidatedPayment(const std::string& prompt = "Enter monthly payment (PKR): ", int maxRetries = 3);
    static std::string getValidatedInput(const std::string& prompt = "Enter input: ", bool canBeEmpty = false, int maxRetries = 3);
    
    // Silent validation (returns empty string if invalid)
    static bool validateEmailSilent(const std::string& email);
    static bool validatePhoneSilent(const std::string& phone);
    static bool validateDateSilent(const std::string& date);
    static bool validateCNICSilent(const std::string& cnic);

private:
    static void printError(const std::string& errorMessage);
    static void printInfo(const std::string& message);
};
