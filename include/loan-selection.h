#pragma once
#include<vector>
#include<string>
#include"home-loan.h"
class LoanSelection {
private:
    std::vector<HomeLoan> _home_loans;
public:
    LoanSelection();
    LoanSelection(const std::string &file_name);
    bool storeHomeLoans(const std::string &file_name);
    std::vector<HomeLoan> homeLoansInArea(const std::string &utterance);
    bool selectHome();
    bool payPerMonth();
    bool monthlyInstallment();
    std::string calculateMonthlyPayment(const HomeLoan &loan) const;
    void calculateInstallmentBreakdown(const HomeLoan &loan, std::vector<std::string> &installments, std::vector<std::string> &remaining_price, std::string &total_price, std::string &down_payment) const;
};
