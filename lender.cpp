#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include "../include/application-handler.h"

// Helper to safely clear cin on bad input
void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printApplication(const Application& app) {
    std::cout << "\n=== APPLICATION DETAILS (ID: " << app.app_id << ") ===\n";
    std::cout << "Loan Type: " << app.loan_type << " (" << app.sub_type << " - Plan " << app.plan_id << ")\n";
    std::cout << "Full Name: " << app.full_name << "\n";
    std::cout << "Father's Name: " << app.father_name << "\n";
    std::cout << "Postal Address: " << app.postal_address << "\n";
    std::cout << "Contact Number: " << app.contact_number << "\n";
    std::cout << "Email: " << app.email_address << "\n";
    std::cout << "CNIC: " << app.cnic_number << "\n";
    std::cout << "CNIC Expiry: " << app.cnic_expiry << "\n";
    std::cout << "Employment: " << app.employment_status << "\n";
    std::cout << "Marital: " << app.marital_status << "\n";
    std::cout << "Gender: " << app.gender << "\n";
    std::cout << "Dependents: " << app.num_dependents << "\n";
    std::cout << "Annual Income: PKR " << app.annual_income << "\n";
    std::cout << "Avg Electricity: " << app.avg_electricity << " units\n";
    std::cout << "Current Electricity: " << app.current_electricity << " units\n";

    // Existing loans printing
    std::cout << "Existing Loans:\n";
    if (app.existing_loans == "no") {
        std::cout << "  None\n";
    } else if (app.existing_loans.find("yes") == 0) {
        std::string loans_str = app.existing_loans.substr(3);
        std::stringstream ss(loans_str);
        std::string loan;
        int loan_num = 1;
        while (std::getline(ss, loan, '|')) {
            if (loan.empty()) continue;
            std::stringstream loan_ss(loan);
            std::string active, total, returned, due, bank, category;
            std::getline(loan_ss, active, ',');
            std::getline(loan_ss, total, ',');
            std::getline(loan_ss, returned, ',');
            std::getline(loan_ss, due, ',');
            std::getline(loan_ss, bank, ',');
            std::getline(loan_ss, category, ',');
            std::cout << "  Loan " << loan_num++ << ":\n";
            std::cout << "    Active: " << active << "\n";
            std::cout << "    Total: PKR " << total << "\n";
            std::cout << "    Returned: PKR " << returned << "\n";
            std::cout << "    Due: PKR " << due << "\n";
            std::cout << "    Bank: " << bank << "\n";
            std::cout << "    Category: " << category << "\n";
        }
    } else {
        std::cout << "  " << app.existing_loans << "\n";
    }

    std::cout << "Ref1 Name: " << app.ref1_name << "\n";
    std::cout << "Ref1 CNIC: " << app.ref1_cnic << "\n";
    std::cout << "Ref1 Issue: " << app.ref1_issue << "\n";
    std::cout << "Ref1 Phone: " << app.ref1_phone << "\n";
    std::cout << "Ref1 Email: " << app.ref1_email << "\n";
    std::cout << "Ref2 Name: " << app.ref2_name << "\n";
    std::cout << "Ref2 CNIC: " << app.ref2_cnic << "\n";
    std::cout << "Ref2 Issue: " << app.ref2_issue << "\n";
    std::cout << "Ref2 Phone: " << app.ref2_phone << "\n";
    std::cout << "Ref2 Email: " << app.ref2_email << "\n";
    std::cout << "CNIC Front: " << app.cnic_front << "\n";
    std::cout << "CNIC Back: " << app.cnic_back << "\n";
    std::cout << "Electricity Bill: " << app.elec_bill << "\n";
    std::cout << "Salary Slip: " << app.salary_slip << "\n";
    std::cout << "Status: " << app.status << "\n";
    std::cout << "Starting Month: " << (app.starting_month.empty() ? "Not set" : app.starting_month) << "\n";
    std::cout << "========================================\n";
}

int main() {
    ApplicationHandler handler;
    handler.load("data/applications.txt");

    int choice;
    while (true) {
        std::cout << "\n=== Lender Portal ===\n";
        std::cout << "1. List applications\n";
        std::cout << "2. View details\n";
        std::cout << "3. Approve\n";
        std::cout << "4. Reject\n";
        std::cout << "5. Exit\n";
        std::cout << "Choose: ";

        // Robust menu input
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input! Please enter a number.\n";
            clearInput();
            continue;
        }
        clearInput();  // Always clear buffer after reading number

        if (choice == 5) {
            std::cout << "Goodbye!\n";
            break;
        }

        if (choice == 1) {
            const auto& apps = handler.getApplications();
            if (apps.empty()) {
                std::cout << "No applications found.\n";
            } else {
                std::cout << "\nList of Applications:\n";
                for (const auto& app : apps) {
                    std::cout << app.app_id << " - " << app.full_name 
                              << " (" << app.cnic_number << ") - Status: " << app.status << "\n";
                }
            }

        } else if (choice == 2 || choice == 3 || choice == 4) {
            std::string id;
            std::cout << "Enter ID (e.g., 0001): ";
            std::getline(std::cin, id);

            if (id.empty()) {
                std::cout << "ID cannot be empty!\n";
                continue;
            }

            Application* app = handler.getById(id);
            if (!app) {
                std::cout << "Application not found! Make sure ID is correct (e.g., 0001).\n";
                continue;
            }

            if (choice == 2) {
                printApplication(*app);
            } else {
                if (app->status != "submitted") {
                    std::cout << "Cannot update - status is already '" << app->status << "'.\n";
                } else {
                    app->status = (choice == 3) ? "approved" : "rejected";
                    handler.update(*app);
                    handler.save("data/applications.txt");
                    std::cout << "Application " << id << " is now " << app->status << "!\n";
                }
            }
        } else {
            std::cout << "Invalid choice. Please enter 1-5.\n";
        }
    }
    return 0;
}
