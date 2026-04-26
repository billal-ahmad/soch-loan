#include "../include/display.h"
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <thread>
#include <chrono>

// -------------------- COLOR + ANIMATION HELPERS --------------------
void Display::setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void Display::typeText(const std::string &text, int speed) {
    for (char c : text) {
        std::cout << c;
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }
}

void Display::boxTop() {
    std::cout << "+------------------------------------------------------------+\n";
}

void Display::boxBottom() {
    std::cout << "+------------------------------------------------------------+\n";
}

// -------------------- GREETING --------------------

void Display::showGreetingBanner() {
    setColor(11);
    boxTop();
    std::cout << "║";
    setColor(14);
    std::cout << std::setw(34) << "WELCOME TO SOCH LOAN";
    setColor(11);
    std::cout << std::setw(24) << "║\n";
    boxBottom();
    setColor(7);
}


void Display::greetingResponse(const std::string &utterance) {
    setColor(10);
    typeText("\n" + utterance + "\n\n");
    setColor(7);
}


// -------------------- UNDEFINED INPUT --------------------
void Display::undefinedInputResponse(const std::string &utterance) {
    setColor(12);
    typeText(utterance + "\n");
    setColor(7);
}

// -------------------- LOAN TYPES --------------------
void Display::displayLoanTypes(const std::string &utterance) {
    setColor(14);
    std::cout << "\nAvailable Loan Types:\n\n";
    setColor(11);
    std::cout << "+---------------+---------------------------+\n";
    std::cout << "| Loan Type     | Description               |\n";
    std::cout << "+---------------+---------------------------+\n";
    std::cout << "| Home Loan     | For buying a house       |\n";
    std::cout << "| Car Loan      | For purchasing a vehicle |\n";
    std::cout << "| Bike Loan     | For electric/petrol bikes|\n";
    std::cout << "+---------------+---------------------------+\n";
    setColor(10);
    typeText("\nPlease select a loan type...\n");
    setColor(7);
}

// -------------------- AREA SELECTION --------------------
void Display::selectAreaDisplay(const std::string &utterance) {
    setColor(11);
    std::cout << "\nSelect Area Category:\n\n";
    setColor(14);
    std::cout << "+-------------+------------+\n";
    std::cout << "| Category    | Example    |\n";
    std::cout << "+-------------+------------+\n";
    std::cout << "| City Area   | Lahore     |\n";
    std::cout << "| Suburban    | Bahria Town|\n";
    std::cout << "| Rural       | Village    |\n";
    std::cout << "+-------------+------------+\n";
    setColor(10);
    typeText("\nEnter your preferred area...\n");
    setColor(7);
}
// -------------------- HOME LOAN --------------------
void Display::homeLoanDisplay(
    const std::vector<HomeLoan> &home_loan, 
    int start, 
    int end, 
    const std::string &prompt
) {
    if (home_loan.empty()) {
        setColor(12);
        typeText("No home loan plans available!\n");
        setColor(7);
        return;
    }

    // Fix index range
    if (start < 0) start = 0;
    if (end >= static_cast<int>(home_loan.size()) || end == -1) end = home_loan.size() - 1;
    if (start > end) start = 0;

    setColor(11);
    typeText("\nAVAILABLE HOME LOAN PLANS:\n\n");

    setColor(14);
    // Table header
    std::cout << "+----+------------+---------+------------+--------------+\n";
    std::cout << "| ID | Area       | Size    | Price      | Installments |\n";
    std::cout << "+----+------------+---------+------------+--------------+\n";

    // Table rows
    for (int i = start; i <= end; i++) {
        std::cout << "| "
                  << std::setw(2) << (i + 1) << " | "
                  << std::setw(10) << home_loan[i].getArea() << " | "
                  << std::setw(7) << home_loan[i].getSize() << " | "
                  << std::setw(10) << home_loan[i].getPrice() << " | "
                  << std::setw(12) << home_loan[i].getInstallments() << " |\n";
    }

    std::cout << "+----+------------+---------+------------+--------------+\n";

    setColor(10);
    // Print prompt only once
    if (!prompt.empty()) {
        typeText("\n" + prompt + "\n");
    } else {
        typeText("\nEnter the ID of the plan you want to view...\n");
    }
    setColor(7);
}



// -------------------- OTHER FUNCTIONS --------------------
void Display::selectHomeDisplay() {
    setColor(14);
    typeText("\nEnter Home Plan Number: ");
    setColor(7);
}

void Display::payPerMonthDisplay(const std::string &payment_info) {
    setColor(11);
    std::cout << "\nMonthly Payment:\n\n";
    setColor(10);
    typeText(payment_info + "\n");
    setColor(7);
}

void Display::isInstallmentPlanNeed(const std::string &prompt) {
    setColor(14);
    typeText("\n" + prompt + ": ");
    setColor(7);
}

void Display::monthlyInstallmentDisplay(
    const std::string &total_price,
    const std::string &down_payment,
    const std::vector<std::string> &installments,
    const std::vector<std::string> &remaining_price)
{
    setColor(11);
    std::cout << "\nINSTALLMENT BREAKDOWN\n\n";
    setColor(14);
    std::cout << "+----------------+--------------------+\n";
    std::cout << "| Total Price    | PKR " << std::setw(15) << total_price << " |\n";
    std::cout << "| Down Payment   | PKR " << std::setw(15) << down_payment << " |\n";
    std::cout << "+----------------+--------------------+\n\n";

    std::cout << "+-------+-----------------+-----------------+\n";
    std::cout << "| Month | Installment (PKR)| Remaining (PKR) |\n";
    std::cout << "+-------+-----------------+-----------------+\n";

    for (size_t i = 0; i < installments.size(); i++) {
        std::cout << "| " << std::setw(5) << (i + 1)
                  << " | " << std::setw(15) << installments[i]
                  << " | " << std::setw(15) << remaining_price[i] << " |\n";
    }

    std::cout << "+-------+-----------------+-----------------+\n";
    setColor(10);
    typeText("\nHope this helps! Let me know if you want another plan.\n");
    setColor(7);
}

void Display::promptForInput(const std::string &text) {
    setColor(14);           // Use bright yellow for prompt
    typeText(text);         // Print the prompt with typewriter effect
    setColor(7);            // Reset to default color
}
void Display::carLoanDisplay(
    const std::vector<CarLoan> &car_loans, 
    int start, 
    int end, 
    const std::string &prompt
) {
    if (car_loans.empty()) {
        setColor(12);
        typeText("No car loan plans available!\n");
        setColor(7);
        return;
    }

    if (start < 0) start = 0;
    if (end >= static_cast<int>(car_loans.size()) || end == -1) end = car_loans.size() - 1;

    setColor(11);
    typeText("\nAVAILABLE CAR LOAN PLANS:\n\n");

    setColor(14);
    std::cout << "+----+---------+---------+---------+------+------+----------------+--------------+\n";
    std::cout << "| ID | Make    | Model   | Engine  | Used | Year | Price          | Installments |\n";
    std::cout << "+----+---------+---------+---------+------+------+----------------+--------------+\n";

    for (int i = start; i <= end; i++) {
        std::cout << "| "
                  << std::setw(2) << (i + 1) << " | "
                  << std::setw(7) << car_loans[i].getMake() << " | "
                  << std::setw(7) << car_loans[i].getModel() << " | "
                  << std::setw(7) << car_loans[i].getEngine() << " | "
                  << std::setw(4) << car_loans[i].getUsed() << " | "
                  << std::setw(4) << car_loans[i].getManufacturingYear() << " | "
                  << std::setw(14) << car_loans[i].getPrice() << " | "
                  << std::setw(12) << car_loans[i].getInstallments() << " |\n";
    }

    std::cout << "+----+---------+---------+---------+------+------+----------------+--------------+\n";

    setColor(10);
    if (!prompt.empty()) {
        typeText("\n" + prompt + "\n");
    } else {
        typeText("\nEnter the ID of the plan you want to view...\n");
    }
    setColor(7);
}
void Display::scooterLoanDisplay(
    const std::vector<ScooterLoan> &scooter_loans, 
    int start, 
    int end, 
    const std::string &prompt
) {
    if (scooter_loans.empty()) {
        setColor(12);
        typeText("No scooter loan plans available!\n");
        setColor(7);
        return;
    }

    if (start < 0) start = 0;
    if (end >= static_cast<int>(scooter_loans.size()) || end == -1) end = scooter_loans.size() - 1;

    setColor(11);
    typeText("\nAVAILABLE SCOOTER LOAN PLANS:\n\n");

    setColor(14);
    std::cout << "+----+---------+---------+----------+---------------+-----------+----------------+--------------+\n";
    std::cout << "| ID | Make    | Model   | Distance | Charging Time | Max Speed | Price          | Installments |\n";
    std::cout << "+----+---------+---------+----------+---------------+-----------+----------------+--------------+\n";

    for (int i = start; i <= end; i++) {
        std::cout << "| "
                  << std::setw(2) << (i + 1) << " | "
                  << std::setw(7) << scooter_loans[i].getMake() << " | "
                  << std::setw(7) << scooter_loans[i].getModel() << " | "
                  << std::setw(8) << scooter_loans[i].getDistance() << " | "
                  << std::setw(13) << scooter_loans[i].getChargingTime() << " | "
                  << std::setw(9) << scooter_loans[i].getMaxSpeed() << " | "
                  << std::setw(14) << scooter_loans[i].getPrice() << " | "
                  << std::setw(12) << scooter_loans[i].getInstallments() << " |\n";
    }

    std::cout << "+----+---------+---------+----------+---------------+-----------+----------------+--------------+\n";

    setColor(10);
    if (!prompt.empty()) {
        typeText("\n" + prompt + "\n");
    } else {
        typeText("\nEnter the ID of the plan you want to view...\n");
    }
    setColor(7);
}
void Display::monthlyPlanDisplay(
    const std::string &total_price,
    const std::string &down_payment,
    const std::vector<std::string> &months,
    const std::vector<std::string> &installments,
    const std::vector<std::string> &remaining_price)
{
    setColor(11);
    std::cout << "\nMONTHLY PAYMENT PLAN\n\n";
    setColor(14);
    std::cout << "+----------------+--------------------+\n";
    std::cout << "| Total Price    | PKR " << std::setw(15) << total_price << " |\n";
    std::cout << "| Down Payment   | PKR " << std::setw(15) << down_payment << " |\n";
    std::cout << "+----------------+--------------------+\n\n";

    std::cout << "+-----------------+-----------------+-----------------+\n";
    std::cout << "| Month           | Installment (PKR)| Remaining (PKR) |\n";
    std::cout << "+-----------------+-----------------+-----------------+\n";

    for (size_t i = 0; i < installments.size(); i++) {
        std::cout << "| " << std::setw(15) << months[i]
                  << " | " << std::setw(15) << installments[i]
                  << " | " << std::setw(15) << remaining_price[i] << " |\n";
    }

    std::cout << "+-----------------+-----------------+-----------------+\n";
    setColor(10);
    typeText("\nHope this helps! Let me know if you need anything else.\n");
    setColor(7);
}

void Display::personalLoanDisplay(
    const std::vector<PersonalLoan> &personal_loans, 
    int start, 
    int end, 
    const std::string &prompt
) {
    if (personal_loans.empty()) {
        setColor(12);
        typeText("No personal loan plans available!\n");
        setColor(7);
        return;
    }

    if (start < 0) start = 0;
    if (end >= static_cast<int>(personal_loans.size()) || end == -1) end = personal_loans.size() - 1;

    setColor(11);
    typeText("\nAVAILABLE PERSONAL LOAN PLANS:\n\n");

    setColor(14);
    std::cout << "+----+------------+------------+--------------+----------------+----------+\n";
    std::cout << "| ID | Category   | Amount     | Installments | Total Payable  | Advance  |\n";
    std::cout << "+----+------------+------------+--------------+----------------+----------+\n";

    for (int i = start; i <= end; i++) {
        std::cout << "| "
                  << std::setw(2) << (i + 1) << " | "
                  << std::setw(10) << personal_loans[i].getCategory() << " | "
                  << std::setw(10) << personal_loans[i].getAmount() << " | "
                  << std::setw(12) << personal_loans[i].getInstallments() << " | "
                  << std::setw(14) << personal_loans[i].getTotalPayable() << " | "
                  << std::setw(8) << personal_loans[i].getAdvance() << " |\n";
    }

    std::cout << "+----+------------+------------+--------------+----------------+----------+\n";

    setColor(10);
    if (!prompt.empty()) {
        typeText("\n" + prompt + "\n");
    } else {
        typeText("\nEnter the ID of the plan you want to view...\n");
    }
    setColor(7);
}
