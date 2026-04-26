#pragma once
#include <string>
#include <vector>

/**
 * ValidationUtils - Centralized validation utilities
 * 
 * This class provides consistent validation methods used throughout the application
 * for dates, emails, phone numbers, CNIC, etc.
 */
class ValidationUtils {
public:
    // Basic type checks
    static bool isDigitOnly(const std::string& str);
    static bool isNumeric(const std::string& str);
    static bool isEmpty(const std::string& str);

    // Specific validations
    static bool isValidDate(const std::string& date);           // Format: DD-MM-YYYY
    static bool isValidEmail(const std::string& email);
    static bool isValidPhoneNumber(const std::string& phone);
    static bool isValidCNIC(const std::string& cnic);           // Pakistan CNIC format
    static bool isValidMonthYear(const std::string& monthYear); // Format: Month YYYY
    
    // Number range validations
    static bool isValidAge(int age);
    static bool isValidIncome(double income);
    static bool isValidMonthlyPayment(double payment);
    
    // Collection validations
    static std::vector<std::string> validateAndSanitize(const std::vector<std::string>& inputs);
    static bool validateDateRange(const std::string& startDate, const std::string& endDate);

private:
    // Helper methods
    static bool isValidMonthName(const std::string& month);
    static bool isValidYear(const std::string& year);
    static bool isValidDayMonthYear(int day, int month, int year);
};
