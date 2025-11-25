#include "../include/personal-loan-selection.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

PersonalLoanSelection::PersonalLoanSelection() {}

PersonalLoanSelection::PersonalLoanSelection(const std::string &file_name) {
    storePersonalLoans(file_name);
}

bool PersonalLoanSelection::storePersonalLoans(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << file_name << std::endl;
        return false;
    }

    _personal_loans.clear();
    std::string line;

    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string category, amount, installmentsStr, total_payable, advance;

        if (std::getline(ss, category, '#') &&
            std::getline(ss, amount, '#') &&
            std::getline(ss, installmentsStr, '#') &&
            std::getline(ss, total_payable, '#') &&
            std::getline(ss, advance, '#')) {

            int installments = std::stoi(installmentsStr);

            _personal_loans.push_back(PersonalLoan(category, amount, installments, total_payable, advance));
        }
    }

    file.close();
    return true;
}

std::vector<PersonalLoan> PersonalLoanSelection::personalsInCategory(const std::string &category) {
    std::string cat_lower = category;
    std::transform(cat_lower.begin(), cat_lower.end(), cat_lower.begin(), ::tolower);
    
    std::vector<PersonalLoan> filtered;
    for (const auto& loan : _personal_loans) {
        std::string loanCat = loan.getCategory();
        std::transform(loanCat.begin(), loanCat.end(), loanCat.begin(), ::tolower);
        if (loanCat.find(cat_lower) != std::string::npos) {
            filtered.push_back(loan);
        }
    }
    return filtered;
}

std::string PersonalLoanSelection::calculateMonthlyPayment(const PersonalLoan &loan) const {
    std::string totalStr = loan.getTotalPayable();
    std::string advanceStr = loan.getAdvance();

    totalStr.erase(std::remove(totalStr.begin(), totalStr.end(), ','), totalStr.end());
    advanceStr.erase(std::remove(advanceStr.begin(), advanceStr.end(), ','), advanceStr.end());

    double total = std::stod(totalStr);
    double advance = std::stod(advanceStr);
    int installments = loan.getInstallments();

    if (installments > 0) {
        double monthly = (total - advance) / installments;
        std::string info = loan.getCategory() + " - Amount " + loan.getAmount() +
                           ": Monthly = " + std::to_string((int)monthly) +
                           " for " + std::to_string(installments) + " months";
        return info;
    }
    return "Calculation error";
}

void PersonalLoanSelection::calculateInstallmentBreakdown(const PersonalLoan &loan,
                                                          std::vector<std::string> &installments,
                                                          std::vector<std::string> &remaining_price,
                                                          std::string &total_price,
                                                          std::string &down_payment) const {
    total_price = loan.getTotalPayable();
    down_payment = loan.getAdvance();

    std::string totalStr = total_price;
    std::string advStr = down_payment;

    totalStr.erase(std::remove(totalStr.begin(), totalStr.end(), ','), totalStr.end());
    advStr.erase(std::remove(advStr.begin(), advStr.end(), ','), advStr.end());

    double total = std::stod(totalStr);
    double adv = std::stod(advStr);
    int numInst = loan.getInstallments();

    if (numInst > 0) {
        double remaining = total - adv;
        double monthly = remaining / numInst;
        for (int m = 1; m <= numInst; ++m) {
            installments.push_back(std::to_string((int)monthly));
            remaining -= monthly;
            remaining_price.push_back(std::to_string((int)remaining));
        }
    }
}
