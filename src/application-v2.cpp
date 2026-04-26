#include "../include/application-v2.h"
#include "../include/validation-utils.h"
#include <sstream>
#include <iostream>
#include <algorithm>

// Date implementation
std::string Date::toString() const {
    char buffer[11];  // DD-MM-YYYY\0
    snprintf(buffer, sizeof(buffer), "%02d-%02d-%04d", day, month, year);
    return std::string(buffer);
}

Date Date::fromString(const std::string& dateStr) {
    if (!ValidationUtils::isValidDate(dateStr)) {
        return Date(0, 0, 0);
    }
    
    int d = std::stoi(dateStr.substr(0, 2));
    int m = std::stoi(dateStr.substr(3, 2));
    int y = std::stoi(dateStr.substr(6, 4));
    
    return Date(d, m, y);
}

bool Date::isValid() const {
    return day >= 1 && day <= 31 &&
           month >= 1 && month <= 12 &&
           year >= 1900 && year <= 2100 &&
           ValidationUtils::isValidDayMonthYear(day, month, year);
}

// ApplicationV2 implementation
ApplicationV2::ApplicationV2()
    : num_dependents(0),
      annual_income(0),
      avg_electricity(0),
      current_electricity(0),
      employment_status(EmploymentStatus::UNEMPLOYED),
      marital_status(MaritalStatus::SINGLE),
      gender(Gender::OTHER),
      status(ApplicationStatus::DRAFT) {}

std::string ApplicationV2::serialize() const {
    std::stringstream ss;
    ss << app_id
       << "#" << loan_type
       << "#" << sub_type
       << "#" << plan_id
       << "#" << full_name
       << "#" << father_name
       << "#" << postal_address
       << "#" << contact_number
       << "#" << email_address
       << "#" << cnic_number
       << "#" << cnic_expiry.toString()
       << "#" << static_cast<int>(employment_status)
       << "#" << static_cast<int>(marital_status)
       << "#" << static_cast<int>(gender)
       << "#" << num_dependents
       << "#" << annual_income
       << "#" << avg_electricity
       << "#" << current_electricity
       << "#" << existing_loans
       << "#" << ref1.name
       << "#" << ref1.cnic
       << "#" << ref1.issue_date.toString()
       << "#" << ref1.phone
       << "#" << ref1.email
       << "#" << ref2.name
       << "#" << ref2.cnic
       << "#" << ref2.issue_date.toString()
       << "#" << ref2.phone
       << "#" << ref2.email
       << "#" << cnic_front_path
       << "#" << cnic_back_path
       << "#" << electricity_bill_path
       << "#" << salary_slip_path
       << "#" << static_cast<int>(status)
       << "#" << starting_month;
    
    return ss.str();
}

void ApplicationV2::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> fields;
    
    while (std::getline(ss, token, '#')) {
        fields.push_back(token);
    }
    
    if (fields.size() < 36) {
        return;  // Invalid format
    }
    
    try {
        size_t idx = 0;
        app_id = fields[idx++];
        loan_type = fields[idx++];
        sub_type = fields[idx++];
        plan_id = fields[idx++];
        full_name = fields[idx++];
        father_name = fields[idx++];
        postal_address = fields[idx++];
        contact_number = fields[idx++];
        email_address = fields[idx++];
        cnic_number = fields[idx++];
        
        std::string dateStr = fields[idx++];
        cnic_expiry = Date::fromString(dateStr);
        
        employment_status = static_cast<EmploymentStatus>(std::stoi(fields[idx++]));
        marital_status = static_cast<MaritalStatus>(std::stoi(fields[idx++]));
        gender = static_cast<Gender>(std::stoi(fields[idx++]));
        num_dependents = std::stoi(fields[idx++]);
        annual_income = std::stoi(fields[idx++]);
        avg_electricity = std::stoi(fields[idx++]);
        current_electricity = std::stoi(fields[idx++]);
        existing_loans = fields[idx++];
        
        ref1.name = fields[idx++];
        ref1.cnic = fields[idx++];
        ref1.issue_date = Date::fromString(fields[idx++]);
        ref1.phone = fields[idx++];
        ref1.email = fields[idx++];
        
        ref2.name = fields[idx++];
        ref2.cnic = fields[idx++];
        ref2.issue_date = Date::fromString(fields[idx++]);
        ref2.phone = fields[idx++];
        ref2.email = fields[idx++];
        
        cnic_front_path = fields[idx++];
        cnic_back_path = fields[idx++];
        electricity_bill_path = fields[idx++];
        salary_slip_path = fields[idx++];
        status = static_cast<ApplicationStatus>(std::stoi(fields[idx++]));
        starting_month = fields[idx++];
    } catch (const std::exception& e) {
        std::cerr << "Error deserializing application: " << e.what() << std::endl;
    }
}

bool ApplicationV2::isValid() const {
    // Check required fields
    if (app_id.empty() || full_name.empty() || email_address.empty() || cnic_number.empty()) {
        return false;
    }
    
    // Validate types
    if (!ValidationUtils::isValidEmail(email_address)) {
        return false;
    }
    
    if (!ValidationUtils::isValidCNIC(cnic_number)) {
        return false;
    }
    
    if (!cnic_expiry.isValid()) {
        return false;
    }
    
    if (num_dependents < 0 || num_dependents > 20) {
        return false;
    }
    
    if (!ValidationUtils::isValidIncome(annual_income)) {
        return false;
    }
    
    return true;
}

std::string ApplicationV2::getValidationErrors() const {
    std::stringstream errors;
    
    if (app_id.empty()) {
        errors << "Application ID is required.\n";
    }
    
    if (full_name.empty()) {
        errors << "Full name is required.\n";
    }
    
    if (email_address.empty()) {
        errors << "Email address is required.\n";
    } else if (!ValidationUtils::isValidEmail(email_address)) {
        errors << "Email address format is invalid.\n";
    }
    
    if (cnic_number.empty()) {
        errors << "CNIC number is required.\n";
    } else if (!ValidationUtils::isValidCNIC(cnic_number)) {
        errors << "CNIC number format is invalid.\n";
    }
    
    if (!cnic_expiry.isValid()) {
        errors << "CNIC expiry date is invalid.\n";
    }
    
    if (num_dependents < 0 || num_dependents > 20) {
        errors << "Number of dependents must be between 0 and 20.\n";
    }
    
    if (!ValidationUtils::isValidIncome(annual_income)) {
        errors << "Annual income must be a positive number.\n";
    }
    
    return errors.str();
}
