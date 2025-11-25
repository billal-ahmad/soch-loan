#include "../include/personal-loan.h"

PersonalLoan::PersonalLoan() : _installments(0) {}

PersonalLoan::PersonalLoan(std::string category, std::string amount, int installments, std::string total_payable, std::string advance)
    : _category(category), _amount(amount), _installments(installments), _total_payable(total_payable), _advance(advance) {}

std::string PersonalLoan::getCategory() const { return _category; }
std::string PersonalLoan::getAmount() const { return _amount; }
int PersonalLoan::getInstallments() const { return _installments; }
std::string PersonalLoan::getTotalPayable() const { return _total_payable; }
std::string PersonalLoan::getAdvance() const { return _advance; }
