#include "../include/validation-utils.h"
#include <cctype>
#include <algorithm>
#include <sstream>
#include <map>

bool ValidationUtils::isDigitOnly(const std::string& str) {
    if (str.empty()) return false;
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

bool ValidationUtils::isNumeric(const std::string& str) {
    if (str.empty()) return false;
    
    // Allow optional leading sign
    size_t start = 0;
    if (str[0] == '+' || str[0] == '-') {
        start = 1;
    }
    
    bool hasDecimal = false;
    for (size_t i = start; i < str.length(); ++i) {
        if (str[i] == '.') {
            if (hasDecimal) return false;  // Multiple decimal points
            hasDecimal = true;
        } else if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return start < str.length();  // Must have at least one digit
}

bool ValidationUtils::isEmpty(const std::string& str) {
    return str.empty() || std::all_of(str.begin(), str.end(), ::isspace);
}

bool ValidationUtils::isValidDate(const std::string& date) {
    // Expected format: DD-MM-YYYY
    if (date.length() != 10 || date[2] != '-' || date[5] != '-') {
        return false;
    }

    std::string dayStr = date.substr(0, 2);
    std::string monthStr = date.substr(3, 2);
    std::string yearStr = date.substr(6, 4);

    if (!isDigitOnly(dayStr) || !isDigitOnly(monthStr) || !isDigitOnly(yearStr)) {
        return false;
    }

    int day = std::stoi(dayStr);
    int month = std::stoi(monthStr);
    int year = std::stoi(yearStr);

    return isValidDayMonthYear(day, month, year);
}

bool ValidationUtils::isValidEmail(const std::string& email) {
    if (email.empty()) return false;
    
    size_t atPos = email.find('@');
    size_t dotPos = email.rfind('.');
    
    if (atPos == std::string::npos || dotPos == std::string::npos) {
        return false;
    }
    
    // Email should have format: localpart@domain.extension
    if (atPos == 0 || dotPos == 0 || atPos >= dotPos) {
        return false;
    }
    
    // Check that @ appears only once
    if (email.find('@', atPos + 1) != std::string::npos) {
        return false;
    }
    
    // Extension should be at least 2 characters
    if (email.length() - dotPos - 1 < 2) {
        return false;
    }
    
    return true;
}

bool ValidationUtils::isValidPhoneNumber(const std::string& phone) {
    if (phone.empty() || phone.length() < 10 || phone.length() > 15) {
        return false;
    }
    return isDigitOnly(phone) || 
           std::all_of(phone.begin(), phone.end(), [](char c) { 
               return std::isdigit(c) || c == '+' || c == '-' || c == ' '; 
           });
}

bool ValidationUtils::isValidCNIC(const std::string& cnic) {
    // Pakistan CNIC format: 5 digits - 7 digits - 1 digit
    if (cnic.length() != 15 || cnic[5] != '-' || cnic[13] != '-') {
        return false;
    }
    
    std::string part1 = cnic.substr(0, 5);
    std::string part2 = cnic.substr(6, 7);
    std::string part3 = cnic.substr(14, 1);
    
    return isDigitOnly(part1) && isDigitOnly(part2) && isDigitOnly(part3);
}

bool ValidationUtils::isValidMonthYear(const std::string& monthYear) {
    // Expected format: "Month YYYY" (e.g., "January 2025")
    size_t spacePos = monthYear.find(' ');
    if (spacePos == std::string::npos || spacePos == 0 || spacePos == monthYear.length() - 1) {
        return false;
    }
    
    std::string month = monthYear.substr(0, spacePos);
    std::string year = monthYear.substr(spacePos + 1);
    
    if (!isValidMonthName(month) || !isValidYear(year)) {
        return false;
    }
    
    return true;
}

bool ValidationUtils::isValidAge(int age) {
    return age >= 18 && age <= 120;
}

bool ValidationUtils::isValidIncome(double income) {
    return income > 0 && income < 1000000000;  // Up to 1 billion
}

bool ValidationUtils::isValidMonthlyPayment(double payment) {
    return payment > 0 && payment < 10000000;  // Up to 10 million
}

std::vector<std::string> ValidationUtils::validateAndSanitize(const std::vector<std::string>& inputs) {
    std::vector<std::string> result;
    for (const auto& input : inputs) {
        if (!isEmpty(input)) {
            result.push_back(input);
        }
    }
    return result;
}

bool ValidationUtils::validateDateRange(const std::string& startDate, const std::string& endDate) {
    if (!isValidDate(startDate) || !isValidDate(endDate)) {
        return false;
    }
    return startDate <= endDate;  // Simple string comparison for YYYY-MM-DD format
}

bool ValidationUtils::isValidMonthName(const std::string& month) {
    static const std::map<std::string, int> monthMap = {
        {"january", 1}, {"february", 2}, {"march", 3}, {"april", 4},
        {"may", 5}, {"june", 6}, {"july", 7}, {"august", 8},
        {"september", 9}, {"october", 10}, {"november", 11}, {"december", 12}
    };
    
    std::string lowerMonth = month;
    std::transform(lowerMonth.begin(), lowerMonth.end(), lowerMonth.begin(), ::tolower);
    
    return monthMap.find(lowerMonth) != monthMap.end();
}

bool ValidationUtils::isValidYear(const std::string& year) {
    if (!isDigitOnly(year) || year.length() != 4) {
        return false;
    }
    int y = std::stoi(year);
    return y >= 1900 && y <= 2100;
}

bool ValidationUtils::isValidDayMonthYear(int day, int month, int year) {
    if (month < 1 || month > 12) return false;
    if (year < 1900 || year > 2100) return false;
    if (day < 1) return false;
    
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Handle leap year
    if (month == 2) {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        return day <= (isLeap ? 29 : 28);
    }
    
    return day <= daysInMonth[month - 1];
}
