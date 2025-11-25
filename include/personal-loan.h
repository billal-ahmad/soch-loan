#pragma once
#include <string>

class PersonalLoan {
private:
    std::string _category;
    std::string _amount;
    int _installments;
    std::string _total_payable;
    std::string _advance;
public:
    PersonalLoan();
    PersonalLoan(std::string category, std::string amount, int installments, std::string total_payable, std::string advance);
    
    std::string getCategory() const;
    std::string getAmount() const;
    int getInstallments() const;
    std::string getTotalPayable() const;
    std::string getAdvance() const;
};
