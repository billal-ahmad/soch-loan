#pragma once
#include <vector>
#include <string>
#include "home-loan.h"
#include "car-loan.h"
#include "scooter-loan.h"
#include "personal-loan.h"

class Display {
public:
    // Helper methods
    void setColor(int color);
    void typeText(const std::string &text, int speed = 30);
    void boxTop();
    void boxBottom();

    // Public display methods
    void showGreetingBanner();
    void greetingResponse(const std::string &utterence);
    void undefinedInputResponse(const std::string &utterence);
    void displyLoanTypes(const std::string &utterence);
    void selectAreaDisplay(const std::string &utterence);
    void homeLoanDisplay(const std::vector<HomeLoan> &home_loan, int start = 0, int end = -1, const std::string &prompt = "");
    void carLoanDisplay(const std::vector<CarLoan> &car_loans, int start = 0, int end = -1, const std::string &prompt = "");
    void scooterLoanDisplay(const std::vector<ScooterLoan> &scooter_loans, int start = 0, int end = -1, const std::string &prompt = "");
    void selectHomeDisplay();
    void payPerMonthDisplay(const std::string &price);
    void isInstallmentPlanNeed(const std::string &prompt = "Do you want to view the installment breakdown? (yes/no)");
    void monthlyInstallmentDisplay(const std::string &total_price, const std::string &down_payment, const std::vector<std::string> &installments, const std::vector<std::string> &remaining_price);
    void monthlyPlanDisplay(const std::string &total_price, const std::string &down_payment, const std::vector<std::string> &months, const std::vector<std::string> &installments, const std::vector<std::string> &remaining_price);
    void promptForInput(const std::string &text);
    void personalLoanDisplay(const std::vector<PersonalLoan> &personal_loans, int start = 0, int end = -1, const std::string &prompt = "");
};
