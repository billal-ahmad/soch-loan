#include "../include/loan-selection.h"
#include "../include/display.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

// Default constructor
LoanSelection::LoanSelection() {}

// Parameterized constructor
LoanSelection::LoanSelection(const std::string &file_name) {
    storeHomeLoans(file_name);
}

bool LoanSelection::storeHomeLoans(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << file_name << std::endl;
        return false;
    }

    _home_loans.clear();
    std::string line;

    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string area, size, installmentsStr, price, down_payment;

        // Parse CSV format with # delimiter
        if (std::getline(ss, area, '#') &&
            std::getline(ss, size, '#') &&
            std::getline(ss, installmentsStr, '#') &&
            std::getline(ss, price, '#') &&
            std::getline(ss, down_payment, '#')) {

            // Convert installments to integer
            int installments = 0;
            try {
                installments = std::stoi(installmentsStr);
            } catch (const std::exception &e) {
                std::cerr << "Error converting installments to integer: " << installmentsStr << std::endl;
                continue;
            }

            // Create HomeLoan object and add to vector
            HomeLoan homeLoan(area, size, installments, price, down_payment);
            _home_loans.push_back(homeLoan);
        }
    }

    file.close();
    return true;
}

std::vector<HomeLoan> LoanSelection::homeLoansInArea(const std::string &utterance) {
    std::string area = utterance;
    std::transform(area.begin(), area.end(), area.begin(), ::tolower);
    
    std::vector<HomeLoan> filteredLoans;

    for (const auto& loan : _home_loans) {
        std::string loanArea = loan.getArea();
        std::transform(loanArea.begin(), loanArea.end(), loanArea.begin(), ::tolower);
        
        if (loanArea.find(area) != std::string::npos) {
            filteredLoans.push_back(loan);
        }
    }

    return filteredLoans;  // Only return — no display here!
}

bool LoanSelection::selectHome() {
    if (_home_loans.empty()) {
        std::cout << "No home loans available. Please load home loans first." << std::endl;
        return false;
    }

    Display display;
    display.homeLoanDisplay(_home_loans, 0, _home_loans.size() - 1);
    return true;
}

bool LoanSelection::payPerMonth() {
    if (_home_loans.empty()) {
        std::cout << "No home loans available for payment calculation." << std::endl;
        return false;
    }

    std::vector<std::string> monthlyPayments;

    // WORKAROUND: Remove const from loop
    for (auto &loan : _home_loans) {
        std::string priceStr = loan.getPrice();
        std::string downPaymentStr = loan.getDownPayment();

        priceStr.erase(std::remove(priceStr.begin(), priceStr.end(), ','), priceStr.end());
        downPaymentStr.erase(std::remove(downPaymentStr.begin(), downPaymentStr.end(), ','), downPaymentStr.end());

        try {
            double price = std::stod(priceStr);
            double downPayment = std::stod(downPaymentStr);
            int installments = loan.getInstallments();
            if (installments > 0) {
                double monthlyInstallment = (price - downPayment) / installments;
                std::string paymentInfo = loan.getArea() + " - " + loan.getSize() +
                                          ": Monthly Installment = " + std::to_string((int)monthlyInstallment) +
                                          " for " + std::to_string(installments) + " months";
                monthlyPayments.push_back(paymentInfo);
            }
        } catch (const std::exception &e) {
            std::cerr << "Error calculating payment for: " << loan.getArea() << std::endl;
            continue;
        }
    }

    std::string allPayments;
    for (size_t i = 0; i < monthlyPayments.size(); ++i) {
        allPayments += monthlyPayments[i];
        if (i < monthlyPayments.size() - 1) allPayments += "\n";
    }

    Display display;
    display.payPerMonthDisplay(allPayments);
    return true;
}

bool LoanSelection::monthlyInstallment() {
    if (_home_loans.empty()) {
        std::cout << "No home loans available for installment plans." << std::endl;
        return false;
    }

    std::vector<std::string> installmentPlans;
    std::vector<std::string> remainingPrices;

    // WORKAROUND: Remove const from loop
    for (auto &loan : _home_loans) {
        std::string priceStr = loan.getPrice();
        std::string downPaymentStr = loan.getDownPayment();

        priceStr.erase(std::remove(priceStr.begin(), priceStr.end(), ','), priceStr.end());
        downPaymentStr.erase(std::remove(downPaymentStr.begin(), downPaymentStr.end(), ','), downPaymentStr.end());

        try {
            double price = std::stod(priceStr);
            double downPayment = std::stod(downPaymentStr);
            int installments = loan.getInstallments();

            if (installments > 0) {
                double monthlyInstallment = (price - downPayment) / installments;
                double remainingPrice = price - downPayment;

                std::string plan = loan.getArea() + " - " + loan.getSize() + "\n" +
                                   "Total Price: " + loan.getPrice() + "\n" +
                                   "Down Payment: " + loan.getDownPayment() + "\n" +
                                   std::to_string(installments) + " months x " +
                                   std::to_string((int)monthlyInstallment) + " per month";

                installmentPlans.push_back(plan);
                std::string remaining = "Remaining after down payment: " + std::to_string((int)remainingPrice);
                remainingPrices.push_back(remaining);
            }
        } catch (const std::exception &e) {
            std::cerr << "Error creating installment plan for: " << loan.getArea() << std::endl;
            continue;
        }
    }

    Display display;
    std::string total_price = "Multiple properties available - see details below";
    std::string down_payment = "Various down payment options available";
    display.monthlyInstallmentDisplay(total_price, down_payment, installmentPlans, remainingPrices);
    return true;
}

std::string LoanSelection::calculateMonthlyPayment(const HomeLoan &loan) const {
    std::string priceStr = loan.getPrice();
    std::string downPaymentStr = loan.getDownPayment();

    priceStr.erase(std::remove(priceStr.begin(), priceStr.end(), ','), priceStr.end());
    downPaymentStr.erase(std::remove(downPaymentStr.begin(), downPaymentStr.end(), ','), downPaymentStr.end());

    try {
        double price = std::stod(priceStr);
        double downPayment = std::stod(downPaymentStr);
        int installments = loan.getInstallments();

        if (installments > 0) {
            double monthlyInstallment = (price - downPayment) / installments;
            std::string paymentInfo = loan.getArea() + " - " + loan.getSize() +
                                      ": Monthly Installment = " + std::to_string((int)monthlyInstallment) +
                                      " for " + std::to_string(installments) + " months";
            return paymentInfo;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error calculating payment for: " << loan.getArea() << std::endl;
    }

    return "Calculation error";
}

void LoanSelection::calculateInstallmentBreakdown(const HomeLoan &loan,
                                                  std::vector<std::string> &installments,
                                                  std::vector<std::string> &remaining_price,
                                                  std::string &total_price,
                                                  std::string &down_payment) const {
    total_price = loan.getPrice();
    down_payment = loan.getDownPayment();

    std::string priceStr = total_price;
    std::string downStr = down_payment;

    priceStr.erase(std::remove(priceStr.begin(), priceStr.end(), ','), priceStr.end());
    downStr.erase(std::remove(downStr.begin(), downStr.end(), ','), downStr.end());

    try {
        double price = std::stod(priceStr);
        double down = std::stod(downStr);
        int numInst = loan.getInstallments();

        if (numInst > 0) {
            double remaining = price - down;
            double monthly = remaining / numInst;
            for (int m = 1; m <= numInst; ++m) {
                installments.push_back(std::to_string((int)monthly));
                remaining -= monthly;
                remaining_price.push_back(std::to_string((int)remaining));
            }
        }
    } catch (const std::exception &e) {
        std::cerr << "Error calculating breakdown for: " << loan.getArea() << std::endl;
    }
}
