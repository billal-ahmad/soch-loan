#include "../include/scooter-loan-selection.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

ScooterLoanSelection::ScooterLoanSelection() {}

ScooterLoanSelection::ScooterLoanSelection(const std::string &file_name) {
    storeScooterLoans(file_name);
}

bool ScooterLoanSelection::storeScooterLoans(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << file_name << std::endl;
        return false;
    }

    _scooter_loans.clear();
    std::string line;

    std::getline(file, line); // Skip header

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string make, model, distance, charging_time, max_speed, installmentsStr, price, down_payment;

        if (std::getline(ss, make, '#') &&
            std::getline(ss, model, '#') &&
            std::getline(ss, distance, '#') &&
            std::getline(ss, charging_time, '#') &&
            std::getline(ss, max_speed, '#') &&
            std::getline(ss, installmentsStr, '#') &&
            std::getline(ss, price, '#') &&
            std::getline(ss, down_payment, '#')) {

            int installments = std::stoi(installmentsStr);

            _scooter_loans.push_back(ScooterLoan(make, model, distance, charging_time, max_speed, installments, price, down_payment));
        }
    }

    file.close();
    return true;
}

std::vector<ScooterLoan> ScooterLoanSelection::scootersInMake(const std::string &utterance) {
    std::string make = utterance;
    std::transform(make.begin(), make.end(), make.begin(), ::tolower);
    
    std::vector<ScooterLoan> filtered;
    for (const auto& loan : _scooter_loans) {
        std::string loanMake = loan.getMake();
        std::transform(loanMake.begin(), loanMake.end(), loanMake.begin(), ::tolower);
        if (loanMake.find(make) != std::string::npos) {
            filtered.push_back(loan);
        }
    }
    return filtered;
}

std::string ScooterLoanSelection::calculateMonthlyPayment(const ScooterLoan &loan) const {
    std::string priceStr = loan.getPrice();
    std::string downPaymentStr = loan.getDownPayment();

    priceStr.erase(std::remove(priceStr.begin(), priceStr.end(), ','), priceStr.end());
    downPaymentStr.erase(std::remove(downPaymentStr.begin(), downPaymentStr.end(), ','), downPaymentStr.end());

    double price = std::stod(priceStr);
    double downPayment = std::stod(downPaymentStr);
    int installments = loan.getInstallments();

    if (installments > 0) {
        double monthlyInstallment = (price - downPayment) / installments;
        std::string paymentInfo = loan.getMake() + " - " + loan.getModel() +
                                  ": Monthly Installment = " + std::to_string((int)monthlyInstallment) +
                                  " for " + std::to_string(installments) + " months";
        return paymentInfo;
    }
    return "Calculation error";
}

void ScooterLoanSelection::calculateInstallmentBreakdown(const ScooterLoan &loan,
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
}
