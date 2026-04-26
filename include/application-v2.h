#pragma once
#include <string>
#include <vector>
#include <ctime>

/**
 * ApplicationV2 - Improved Application class with proper type safety
 * 
 * Replaces Application class with proper types:
 * - Dates are stored as Date structures (day, month, year)
 * - Numeric values use int/double instead of strings
 * - Enums for categorical fields (status, employment, marital, gender)
 */

enum class ApplicationStatus {
    DRAFT,           // C0 - Initial draft
    IN_PROGRESS,     // C1 - Partial submission
    IN_REVIEW,       // C2 - Documents submitted
    SUBMITTED,       // C3 - Complete submission
    APPROVED,        // Approved by lender
    REJECTED,        // Rejected by lender
    PENDING          // Under consideration
};

enum class EmploymentStatus {
    EMPLOYED,
    SELF_EMPLOYED,
    RETIRED,
    UNEMPLOYED,
    STUDENT
};

enum class MaritalStatus {
    SINGLE,
    MARRIED,
    DIVORCED,
    WIDOWED
};

enum class Gender {
    MALE,
    FEMALE,
    OTHER
};

struct Date {
    int day;
    int month;
    int year;
    
    Date() : day(0), month(0), year(0) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}
    
    std::string toString() const;
    static Date fromString(const std::string& dateStr);  // Format: DD-MM-YYYY
    bool isValid() const;
};

class ApplicationV2 {
public:
    // Loan Information
    std::string app_id;
    std::string loan_type;          // home, car, scooter, personal
    std::string sub_type;           // Area, Make, Category, etc.
    std::string plan_id;

    // Personal Information
    std::string full_name;
    std::string father_name;
    std::string postal_address;
    std::string contact_number;
    std::string email_address;
    std::string cnic_number;
    Date cnic_expiry;
    
    // Employment Information
    EmploymentStatus employment_status;
    MaritalStatus marital_status;
    Gender gender;
    int num_dependents;
    int annual_income;              // In PKR
    int avg_electricity;            // In units
    int current_electricity;        // In units
    
    // Existing Loans
    std::string existing_loans;     // JSON-like format or structured
    
    // References
    struct Reference {
        std::string name;
        std::string cnic;
        Date issue_date;
        std::string phone;
        std::string email;
    };
    Reference ref1;
    Reference ref2;
    
    // Documents
    std::string cnic_front_path;
    std::string cnic_back_path;
    std::string electricity_bill_path;
    std::string salary_slip_path;
    
    // Application Status
    ApplicationStatus status;
    Date submitted_date;
    std::string starting_month;
    
    ApplicationV2();
    
    // Serialization (compatible with old format)
    std::string serialize() const;
    void deserialize(const std::string& line);
    
    // Validation
    bool isValid() const;
    std::string getValidationErrors() const;
};
