#pragma once
#include <vector>
#include <string>
#include "personal-loan.h"

class PersonalLoanSelection {
private:
    std::vector<PersonalLoan> _personal_loans;
public:
    PersonalLoanSelection();
    PersonalLoanSelection(const std::string &file_name);
    bool storePersonalLoans(const std::string &file_name);
    std::vector<PersonalLoan> personalsInCategory(const std::string &category);
    std::string calculateMonthlyPayment(const PersonalLoan &loan) const;
    void calculateInstallmentBreakdown(const PersonalLoan &loan, std::vector<std::string> &installments, std::vector<std::string> &remaining_price, std::string &total_price, std::string &down_payment) const;
};
