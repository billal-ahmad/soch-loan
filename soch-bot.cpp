#include <iostream>
#include <fstream>
#include <string>
#include "utterance-handler.h"
#include "loan-selection.h"
#include "display.h"
#include "home-loan.h"
#include "application-handler.h"
#include "car-loan.h"
#include "car-loan-selection.h"
#include "scooter-loan.h"
#include "scooter-loan-selection.h"
#include "personal-loan.h"
#include "personal-loan-selection.h"
#include "general-chat-handler.h"
#include <map>
#include <algorithm>
#include <cctype>
#include <vector>

int main() {
    // 1. Load utterances from file in data/ folder
    UtteranceHandler utterHandler;
    if (!utterHandler.storeUserInputAndResponse("data/utterances.txt")) {
        std::cerr << "Failed to load data/utterances.txt. Exiting...\n";
        return 1;
    }

    // 2. Load home loans from file in data/ folder
    LoanSelection loanSelection;
    if (!loanSelection.storeHomeLoans("data/home.txt")) {
        std::cerr << "Failed to load data/home.txt. Exiting...\n";
        return 1;
    }

    CarLoanSelection carSelection("data/car.txt");
    ScooterLoanSelection scooterSelection("data/scooter.txt");
    PersonalLoanSelection personalSelection("data/personal.txt"); // Added for personal loans

    ApplicationHandler appHandler;
    appHandler.load("data/applications.txt");

    GeneralChatHandler chatHandler("data/human-chat-corpus.txt"); // Added for general chat

    // 3. Initial greeting
    Display display;
    std::string initial = utterHandler.getResponse("*");
    display.greetingResponse(initial);

    enum class State { NORMAL, SELECT_LOAN_TYPE, SELECT_AREA_HOME, SELECT_PLAN_HOME, CONFIRM_INSTALLMENT_HOME, PROCEED_APPLY, ASK_RESUME, SELECT_MAKE_CAR, SELECT_PLAN_CAR, CONFIRM_INSTALLMENT_CAR, SELECT_MAKE_SCOOTER, SELECT_PLAN_SCOOTER, CONFIRM_INSTALLMENT_SCOOTER, SELECT_CATEGORY_PERSONAL, SELECT_PLAN_PERSONAL, CONFIRM_INSTALLMENT_PERSONAL, COLLECT_FULL_NAME, COLLECT_FATHER_NAME, COLLECT_ADDRESS, COLLECT_CONTACT, COLLECT_EMAIL, COLLECT_CNIC, COLLECT_CNIC_EXPIRY, COLLECT_EMPLOYMENT, COLLECT_MARITAL, COLLECT_GENDER, COLLECT_DEPENDENTS, COLLECT_ANNUAL_INCOME, COLLECT_AVG_ELEC, COLLECT_CURR_ELEC, COLLECT_HAS_EXISTING, COLLECT_NUM_LOANS, COLLECT_LOAN_ACTIVE, COLLECT_LOAN_TOTAL, COLLECT_LOAN_RETURNED, COLLECT_LOAN_DUE, COLLECT_LOAN_BANK, COLLECT_LOAN_CATEGORY, COLLECT_REF1_NAME, COLLECT_REF1_CNIC, COLLECT_REF1_ISSUE, COLLECT_REF1_PHONE, COLLECT_REF1_EMAIL, COLLECT_REF2_NAME, COLLECT_REF2_CNIC, COLLECT_REF2_ISSUE, COLLECT_REF2_PHONE, COLLECT_REF2_EMAIL, COLLECT_CNIC_FRONT, COLLECT_CNIC_BACK, COLLECT_ELEC_BILL, COLLECT_SALARY_SLIP, COLLECT_CONFIRM, COUNT_CNIC, VIEW_PLAN_ID, SET_START_MONTH, RESUME_ID, RESUME_CNIC, GENERAL };
    State currentState = State::NORMAL;

    std::vector<HomeLoan> current_filtered_home;
    std::vector<CarLoan> current_filtered_car;
    std::vector<ScooterLoan> current_filtered_scooter;
    std::vector<PersonalLoan> current_filtered_personal; // Added for personal loans
    HomeLoan selected_home;
    CarLoan selected_car;
    ScooterLoan selected_scooter;
    PersonalLoan selected_personal; // Added for personal loans
    std::string userInput;
    std::string current_loan_type;
    std::string current_sub_type;
    std::string current_plan_id;
    Application current_app;
    int num_existing = 0;
    int current_loan_index = 0;
    std::map<std::string, std::string> current_loan_data;
    std::vector<std::map<std::string, std::string>> existing_loans_vec;
    std::string current_app_id; // for view plan etc.
    std::string resume_id, resume_cnic; // Added for resume

    const std::string month_names[13] = {
        "",
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };

    auto is_digit = [](const std::string &str) {
        if (str.empty()) {
            return false;
        }
        for (char c : str) {
            if (!std::isdigit(c)) {
                return false;
            }
        }
        return true;
    };

    auto validate_date = [](const std::string &date) {
        if (date.length() != 10 || date[2] != '-' || date[5] != '-') {
            return false;
        }
        // Additional checks can be added if needed (e.g., valid day/month)
        return true;
    };

    auto validate_email = [](const std::string &email) {
        return email.find('@') != std::string::npos &&
               email.find('.') != std::string::npos;
    };

    auto generate_months =
        [&](const std::string &start, int num) -> std::vector<std::string> {
        size_t space = start.find(' ');
        if (space == std::string::npos) {
            return {};
        }
        std::string m_str = start.substr(0, space);
        std::string y_str = start.substr(space + 1);
        int year = std::stoi(y_str);
        std::map<std::string, int> month_map = {
            {"january", 1},
            {"february", 2},
            {"march", 3},
            {"april", 4},
            {"may", 5},
            {"june", 6},
            {"july", 7},
            {"august", 8},
            {"september", 9},
            {"october", 10},
            {"november", 11},
            {"december", 12}
        };
        std::string low_m = utterHandler.toLower(m_str);
        auto it = month_map.find(low_m);
        if (it == month_map.end()) {
            return {};
        }
        int month = it->second;
        std::vector<std::string> res;
        for (int i = 0; i < num; ++i) {
            res.push_back(month_names[month] + " " + std::to_string(year));
            ++month;
            if (month == 13) {
                month = 1;
                ++year;
            }
        }
        return res;
    };

    auto generate_plan = [&](const Application& app) -> void {
        std::vector<std::string> inst;
        std::vector<std::string> rem;
        std::string total_price;
        std::string down_payment;
        int installments = 0;
        if (app.loan_type == "home") {
            LoanSelection ls("data/home.txt");
            auto filtered = ls.homeLoansInArea(app.sub_type);
            int idx = std::stoi(app.plan_id) - 1;
            if (idx < 0 || idx >= static_cast<int>(filtered.size())) {
                display.undefinedInputResponse("Invalid plan.");
                return;
            }
            selected_home = filtered[idx];
            ls.calculateInstallmentBreakdown(
                selected_home,
                inst,
                rem,
                total_price,
                down_payment
            );
            installments = selected_home.getInstallments();
        } else if (app.loan_type == "car") {
            CarLoanSelection cs("data/car.txt");
            auto filtered = cs.carsInMake(app.sub_type);
            int idx = std::stoi(app.plan_id) - 1;
            if (idx < 0 || idx >= static_cast<int>(filtered.size())) {
                display.undefinedInputResponse("Invalid plan.");
                return;
            }
            selected_car = filtered[idx];
            cs.calculateInstallmentBreakdown(
                selected_car,
                inst,
                rem,
                total_price,
                down_payment
            );
            installments = selected_car.getInstallments();
        } else if (app.loan_type == "scooter") {
            ScooterLoanSelection ss("data/scooter.txt");
            auto filtered = ss.scootersInMake(app.sub_type);
            int idx = std::stoi(app.plan_id) - 1;
            if (idx < 0 || idx >= static_cast<int>(filtered.size())) {
                display.undefinedInputResponse("Invalid plan.");
                return;
            }
            selected_scooter = filtered[idx];
            ss.calculateInstallmentBreakdown(
                selected_scooter,
                inst,
                rem,
                total_price,
                down_payment
            );
            installments = selected_scooter.getInstallments();
        } else if (app.loan_type == "personal") { // Added for personal
            PersonalLoanSelection ps("data/personal.txt");
            auto filtered = ps.personalsInCategory(app.sub_type);
            int idx = std::stoi(app.plan_id) - 1;
            if (idx < 0 || idx >= static_cast<int>(filtered.size())) {
                display.undefinedInputResponse("Invalid plan.");
                return;
            }
            selected_personal = filtered[idx];
            ps.calculateInstallmentBreakdown(
                selected_personal,
                inst,
                rem,
                total_price,
                down_payment
            );
            installments = selected_personal.getInstallments();
        }
        auto months = generate_months(app.starting_month, installments);
        if (months.empty()) {
            display.undefinedInputResponse("Invalid starting month.");
            return;
        }
        display.monthlyPlanDisplay(total_price, down_payment, months, inst, rem);
    };

    auto save_partial = [&]() -> void {
        if (current_app.app_id.empty()) {
            return;
        }
        std::string new_status = "C0";
        if (currentState >= State::COLLECT_ANNUAL_INCOME) {
            new_status = "C1";
        }
        if (currentState >= State::COLLECT_REF1_NAME) {
            new_status = "C2";
        }
        if (currentState >= State::COLLECT_CNIC_FRONT) {
            new_status = "C3";
        }
        current_app.status = new_status;
        if (appHandler.getById(current_app.app_id)) {
            appHandler.update(current_app);
        } else {
            appHandler.add(current_app);
        }
        appHandler.save("data/applications.txt");
        display.greetingResponse(
            "Progress saved. You can resume later with ID " + current_app.app_id
        );
    };

    auto start_section =
        [&](const std::string &section) {
        display.greetingResponse("Entering " + section + " section.");
    };

    while (true) {
        std::cout << "\nYou: ";
        std::getline(std::cin, userInput);
        std::string originalInput = userInput;
        userInput = utterHandler.trim(userInput);
        userInput = utterHandler.toLower(userInput);
        if (userInput == "x") {
            if (currentState >= State::PROCEED_APPLY &&
                !current_app.app_id.empty()) { // Added save on exit if in app
                save_partial();
            }
            std::string exitMsg = utterHandler.getResponse("exit");
            std::cout << exitMsg << "\n";
            break;
        }
        if (currentState == State::NORMAL) {
            bool handled = false;
            if (userInput == "h" ||
                userInput == "home" ||
                userInput == "home loan") {
                current_loan_type = "home";
                currentState = State::ASK_RESUME;
                display.promptForInput(utterHandler.getResponse("prompt_resume"));
                handled = true;
            } else if (userInput == "c" ||
                       userInput == "car" ||
                       userInput == "car loan") {
                current_loan_type = "car";
                currentState = State::ASK_RESUME;
                display.promptForInput(utterHandler.getResponse("prompt_resume"));
                handled = true;
            } else if (userInput == "s" ||
                       userInput == "scooter" ||
                       userInput == "scooter loan") {
                current_loan_type = "scooter";
                currentState = State::ASK_RESUME;
                display.promptForInput(utterHandler.getResponse("prompt_resume"));
                handled = true;
            } else if (userInput == "p" ||
                       userInput == "personal" ||
                       userInput == "personal loan") {
                current_loan_type = "personal";
                currentState = State::ASK_RESUME;
                display.promptForInput(utterHandler.getResponse("prompt_resume"));
                handled = true;
            }
            if (!handled) {
                std::string response = utterHandler.generateResponse(originalInput);
                if (response.empty()) {
                    response = chatHandler.getResponse(
                        originalInput
                    ); // Fallback to human-chat-corpus for undefined
                }
                // Show banner only on real greetings
                if (userInput == "hi" || userInput == "hello" || userInput == "*") {
                    display.showGreetingBanner();
                }
                display.greetingResponse(response);
                if (userInput == "a") {
                    currentState = State::SELECT_LOAN_TYPE;
                } else if (userInput == "resume application") { // Added resume handling
                    currentState = State::RESUME_ID;
                    display.promptForInput(
                        utterHandler.getResponse("prompt_resume_id")
                    );
                } else if (userInput == "general chat") { // Added general chat
                    std::string resp = utterHandler.getResponse("enter_general");
                    display.greetingResponse(resp);
                    currentState = State::GENERAL;
                } else if (userInput == "count applications") {
                    currentState = State::COUNT_CNIC;
                    display.promptForInput(
                        utterHandler.getResponse("count applications")
                    );
                } else if (userInput == "view plan") {
                    currentState = State::VIEW_PLAN_ID;
                    display.promptForInput(
                        utterHandler.getResponse("view plan")
                    );
                }
            }
        } else if (currentState == State::ASK_RESUME) { // Added ask resume
            std::string low = utterHandler.toLower(userInput);
            if (low == "yes" || low == "y") {
                currentState = State::RESUME_ID;
                display.promptForInput(
                    utterHandler.getResponse("prompt_resume_id")
                );
            } else if (low == "no" || low == "n") {
                std::string key;
                if (current_loan_type == "home") key = "h";
                else if (current_loan_type == "car") key = "c";
                else if (current_loan_type == "scooter") key = "s";
                else if (current_loan_type == "personal") key = "p";
                std::string resp = utterHandler.generateResponse(key);
                display.greetingResponse(resp);
                if (current_loan_type == "home") {
                    currentState = State::SELECT_AREA_HOME;
                } else if (current_loan_type == "car") {
                    currentState = State::SELECT_MAKE_CAR;
                } else if (current_loan_type == "scooter") {
                    currentState = State::SELECT_MAKE_SCOOTER;
                } else if (current_loan_type == "personal") {
                    currentState = State::SELECT_CATEGORY_PERSONAL;
                }
            } else {
                std::string invalid =
                    utterHandler.getResponse("invalid_yes_no");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::SELECT_LOAN_TYPE) {
            if (userInput == "h") {
                current_loan_type = "home";
                currentState = State::ASK_RESUME;
                display.promptForInput(utterHandler.getResponse("prompt_resume"));
            } else if (userInput == "c") {
                current_loan_type = "car";
                currentState = State::ASK_RESUME;
                display.promptForInput(utterHandler.getResponse("prompt_resume"));
            } else if (userInput == "s") {
                current_loan_type = "scooter";
                currentState = State::ASK_RESUME;
                display.promptForInput(utterHandler.getResponse("prompt_resume"));
            } else if (userInput == "p") { // Added personal in loan type
                current_loan_type = "personal";
                currentState = State::ASK_RESUME;
                display.promptForInput(utterHandler.getResponse("prompt_resume"));
            } else {
                std::string resp = utterHandler.getResponse("invalid_loan_type");
                display.greetingResponse(resp);
            }
        } else if (currentState == State::SELECT_AREA_HOME) {
            int areaNum = -1;
            if (is_digit(userInput)) {
                areaNum = std::stoi(userInput);
            }
            if (areaNum >= 1 && areaNum <= 4) {
                std::string key = "select_area" + std::to_string(areaNum);
                std::string resp = utterHandler.getResponse(key);
                display.greetingResponse(resp);
                current_sub_type = "Area " + std::to_string(areaNum);
                current_filtered_home =
                    loanSelection.homeLoansInArea(current_sub_type);
                if (current_filtered_home.empty()) {
                    std::string noLoans = utterHandler.getResponse("no_loans");
                    display.undefinedInputResponse(noLoans);
                    currentState = State::NORMAL;
                } else {
                    std::string prompt =
                        utterHandler.getResponse("prompt_select_plan");
                    display.homeLoanDisplay(
                        current_filtered_home,
                        0,
                        current_filtered_home.size() - 1,
                        prompt
                    );
                    currentState = State::SELECT_PLAN_HOME;
                }
            } else {
                std::string invalid = utterHandler.getResponse("invalid_area");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::SELECT_MAKE_CAR) {
            int makeNum = -1;
            if (is_digit(userInput)) {
                makeNum = std::stoi(userInput);
            }
            if (makeNum >= 1 && makeNum <= 2) {
                std::string key = "select_make" + std::to_string(makeNum);
                std::string resp = utterHandler.getResponse(key);
                display.greetingResponse(resp);
                current_sub_type = "Make " + std::to_string(makeNum);
                current_filtered_car =
                    carSelection.carsInMake(current_sub_type);
                if (current_filtered_car.empty()) {
                    std::string noLoans = utterHandler.getResponse("no_loans");
                    display.undefinedInputResponse(noLoans);
                    currentState = State::NORMAL;
                } else {
                    std::string prompt =
                        utterHandler.getResponse("prompt_select_plan");
                    display.carLoanDisplay(
                        current_filtered_car,
                        0,
                        current_filtered_car.size() - 1,
                        prompt
                    );
                    currentState = State::SELECT_PLAN_CAR;
                }
            } else {
                std::string invalid = utterHandler.getResponse("invalid_make");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::SELECT_MAKE_SCOOTER) {
            int makeNum = -1;
            if (is_digit(userInput)) {
                makeNum = std::stoi(userInput);
            }
            if (makeNum == 1) {
                std::string key = "select_make" + std::to_string(makeNum);
                std::string resp = utterHandler.getResponse(key);
                display.greetingResponse(resp);
                current_sub_type = "Make " + std::to_string(makeNum);
                current_filtered_scooter =
                    scooterSelection.scootersInMake(current_sub_type);
                if (current_filtered_scooter.empty()) {
                    std::string noLoans = utterHandler.getResponse("no_loans");
                    display.undefinedInputResponse(noLoans);
                    currentState = State::NORMAL;
                } else {
                    std::string prompt =
                        utterHandler.getResponse("prompt_select_plan");
                    display.scooterLoanDisplay(
                        current_filtered_scooter,
                        0,
                        current_filtered_scooter.size() - 1,
                        prompt
                    );
                    currentState = State::SELECT_PLAN_SCOOTER;
                }
            } else {
                std::string invalid = utterHandler.getResponse("invalid_make");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::SELECT_CATEGORY_PERSONAL) { // Added for personal category
            int catNum = -1;
            if (is_digit(userInput)) {
                catNum = std::stoi(userInput);
            }
            if (catNum >= 1 && catNum <= 2) { // Assuming 2 categories; adjust if more
                std::string key = "select_category" + std::to_string(catNum);
                std::string resp = utterHandler.getResponse(key);
                display.greetingResponse(resp);
                current_sub_type = "Category " + std::to_string(catNum);
                current_filtered_personal =
                    personalSelection.personalsInCategory(current_sub_type);
                if (current_filtered_personal.empty()) {
                    std::string noLoans = utterHandler.getResponse("no_loans");
                    display.undefinedInputResponse(noLoans);
                    currentState = State::NORMAL;
                } else {
                    std::string prompt =
                        utterHandler.getResponse("prompt_select_plan");
                    display.personalLoanDisplay(
                        current_filtered_personal,
                        0,
                        current_filtered_personal.size() - 1,
                        prompt
                    );
                    currentState = State::SELECT_PLAN_PERSONAL;
                }
            } else {
                std::string invalid =
                    utterHandler.getResponse("invalid_category");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::SELECT_PLAN_HOME) {
            int id = -1;
            if (is_digit(userInput)) {
                id = std::stoi(userInput);
            }
            if (id >= 1 &&
                id <= static_cast<int>(current_filtered_home.size())) {
                current_plan_id = userInput;
                std::string key = "selected_plan";
                std::string resp = utterHandler.getResponse(key);
                resp = utterHandler.replacePlaceholder(resp, "{id}", userInput);
                display.greetingResponse(resp);
                selected_home = current_filtered_home[id - 1];
                std::string monthlyInfo =
                    loanSelection.calculateMonthlyPayment(selected_home);
                display.payPerMonthDisplay(monthlyInfo);
                std::string promptInst =
                    utterHandler.getResponse("prompt_installment");
                display.promptForInput(promptInst);
                currentState = State::CONFIRM_INSTALLMENT_HOME;
            } else {
                std::string invalid = utterHandler.getResponse("invalid_plan");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::SELECT_PLAN_CAR) {
            int id = -1;
            if (is_digit(userInput)) {
                id = std::stoi(userInput);
            }
            if (id >= 1 &&
                id <= static_cast<int>(current_filtered_car.size())) {
                current_plan_id = userInput;
                std::string key = "selected_plan";
                std::string resp = utterHandler.getResponse(key);
                resp = utterHandler.replacePlaceholder(resp, "{id}", userInput);
                display.greetingResponse(resp);
                selected_car = current_filtered_car[id - 1];
                std::string monthlyInfo =
                    carSelection.calculateMonthlyPayment(selected_car);
                display.payPerMonthDisplay(monthlyInfo);
                std::string promptInst =
                    utterHandler.getResponse("prompt_installment");
                display.promptForInput(promptInst);
                currentState = State::CONFIRM_INSTALLMENT_CAR;
            } else {
                std::string invalid = utterHandler.getResponse("invalid_plan");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::SELECT_PLAN_SCOOTER) {
            int id = -1;
            if (is_digit(userInput)) {
                id = std::stoi(userInput);
            }
            if (id >= 1 &&
                id <= static_cast<int>(current_filtered_scooter.size())) {
                current_plan_id = userInput;
                std::string key = "selected_plan";
                std::string resp = utterHandler.getResponse(key);
                resp = utterHandler.replacePlaceholder(resp, "{id}", userInput);
                display.greetingResponse(resp);
                selected_scooter = current_filtered_scooter[id - 1];
                std::string monthlyInfo =
                    scooterSelection.calculateMonthlyPayment(selected_scooter);
                display.payPerMonthDisplay(monthlyInfo);
                std::string promptInst =
                    utterHandler.getResponse("prompt_installment");
                display.promptForInput(promptInst);
                currentState = State::CONFIRM_INSTALLMENT_SCOOTER;
            } else {
                std::string invalid = utterHandler.getResponse("invalid_plan");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::SELECT_PLAN_PERSONAL) {
            int id = -1;
            if (is_digit(userInput)) {
                id = std::stoi(userInput);
            }
            if (id >= 1 &&
                id <= static_cast<int>(current_filtered_personal.size())) {
                current_plan_id = userInput;
                std::string key = "selected_plan";
                std::string resp = utterHandler.getResponse(key);
                resp = utterHandler.replacePlaceholder(resp, "{id}", userInput);
                display.greetingResponse(resp);
                selected_personal = current_filtered_personal[id - 1];
                std::string monthlyInfo =
                    personalSelection.calculateMonthlyPayment(selected_personal);
                display.payPerMonthDisplay(monthlyInfo);
                std::string promptInst =
                    utterHandler.getResponse("prompt_installment");
                display.promptForInput(promptInst);
                currentState = State::CONFIRM_INSTALLMENT_PERSONAL;
            } else {
                std::string invalid = utterHandler.getResponse("invalid_plan");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::CONFIRM_INSTALLMENT_HOME) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "no" || low == "n") {
                std::string promptApply =
                    utterHandler.getResponse("prompt_apply");
                display.promptForInput(promptApply);
                currentState = State::PROCEED_APPLY;
            } else {
                // Validate month input for breakdown
                size_t space = userInput.find(' ');
                if (space == std::string::npos) {
                    display.greetingResponse(
                        utterHandler.getResponse("invalid_month")
                    );
                    continue;
                }
                std::string m_str = userInput.substr(0, space);
                std::string y_str = userInput.substr(space + 1);
                std::string low_m = utterHandler.toLower(m_str);
                std::map<std::string, int> month_map = {
                    {"january", 1},
                    {"february", 2},
                    {"march", 3},
                    {"april", 4},
                    {"may", 5},
                    {"june", 6},
                    {"july", 7},
                    {"august", 8},
                    {"september", 9},
                    {"october", 10},
                    {"november", 11},
                    {"december", 12}
                };
                if (month_map.find(low_m) == month_map.end() ||
                    !is_digit(y_str)) {
                    display.greetingResponse(
                        utterHandler.getResponse("invalid_month")
                    );
                    continue;
                }
                std::vector<std::string> insts;
                std::vector<std::string> rems;
                std::string total;
                std::string down;
                loanSelection.calculateInstallmentBreakdown(
                    selected_home,
                    insts,
                    rems,
                    total,
                    down
                );
                auto months = generate_months(userInput, insts.size());
                display.monthlyPlanDisplay(
                    total,
                    down,
                    months,
                    insts,
                    rems
                );
                std::string promptApply =
                    utterHandler.getResponse("prompt_apply");
                display.promptForInput(promptApply);
                currentState = State::PROCEED_APPLY;
            }
        } else if (currentState == State::CONFIRM_INSTALLMENT_CAR) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "no" || low == "n") {
                std::string promptApply =
                    utterHandler.getResponse("prompt_apply");
                display.promptForInput(promptApply);
                currentState = State::PROCEED_APPLY;
            } else {
                // Validate month input for breakdown
                size_t space = userInput.find(' ');
                if (space == std::string::npos) {
                    display.greetingResponse(
                        utterHandler.getResponse("invalid_month")
                    );
                    continue;
                }
                std::string m_str = userInput.substr(0, space);
                std::string y_str = userInput.substr(space + 1);
                std::string low_m = utterHandler.toLower(m_str);
                std::map<std::string, int> month_map = {
                    {"january", 1},
                    {"february", 2},
                    {"march", 3},
                    {"april", 4},
                    {"may", 5},
                    {"june", 6},
                    {"july", 7},
                    {"august", 8},
                    {"september", 9},
                    {"october", 10},
                    {"november", 11},
                    {"december", 12}
                };
                if (month_map.find(low_m) == month_map.end() ||
                    !is_digit(y_str)) {
                    display.greetingResponse(
                        utterHandler.getResponse("invalid_month")
                    );
                    continue;
                }
                std::vector<std::string> insts;
                std::vector<std::string> rems;
                std::string total;
                std::string down;
                carSelection.calculateInstallmentBreakdown(
                    selected_car,
                    insts,
                    rems,
                    total,
                    down
                );
                auto months = generate_months(userInput, insts.size());
                display.monthlyPlanDisplay(
                    total,
                    down,
                    months,
                    insts,
                    rems
                );
                std::string promptApply =
                    utterHandler.getResponse("prompt_apply");
                display.promptForInput(promptApply);
                currentState = State::PROCEED_APPLY;
            }
        } else if (currentState == State::CONFIRM_INSTALLMENT_SCOOTER) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "no" || low == "n") {
                std::string promptApply =
                    utterHandler.getResponse("prompt_apply");
                display.promptForInput(promptApply);
                currentState = State::PROCEED_APPLY;
            } else {
                // Validate month input for breakdown
                size_t space = userInput.find(' ');
                if (space == std::string::npos) {
                    display.greetingResponse(
                        utterHandler.getResponse("invalid_month")
                    );
                    continue;
                }
                std::string m_str = userInput.substr(0, space);
                std::string y_str = userInput.substr(space + 1);
                std::string low_m = utterHandler.toLower(m_str);
                std::map<std::string, int> month_map = {
                    {"january", 1},
                    {"february", 2},
                    {"march", 3},
                    {"april", 4},
                    {"may", 5},
                    {"june", 6},
                    {"july", 7},
                    {"august", 8},
                    {"september", 9},
                    {"october", 10},
                    {"november", 11},
                    {"december", 12}
                };
                if (month_map.find(low_m) == month_map.end() ||
                    !is_digit(y_str)) {
                    display.greetingResponse(
                        utterHandler.getResponse("invalid_month")
                    );
                    continue;
                }
                std::vector<std::string> insts;
                std::vector<std::string> rems;
                std::string total;
                std::string down;
                scooterSelection.calculateInstallmentBreakdown(
                    selected_scooter,
                    insts,
                    rems,
                    total,
                    down
                );
                auto months = generate_months(userInput, insts.size());
                display.monthlyPlanDisplay(
                    total,
                    down,
                    months,
                    insts,
                    rems
                );
                std::string promptApply =
                    utterHandler.getResponse("prompt_apply");
                display.promptForInput(promptApply);
                currentState = State::PROCEED_APPLY;
            }
        } else if (currentState == State::CONFIRM_INSTALLMENT_PERSONAL) { // Added for personal installment
            std::string low = utterHandler.toLower(userInput);
            if (low == "no" || low == "n") {
                std::string promptApply =
                    utterHandler.getResponse("prompt_apply");
                display.promptForInput(promptApply);
                currentState = State::PROCEED_APPLY;
            } else {
                // Validate month input for breakdown
                size_t space = userInput.find(' ');
                if (space == std::string::npos) {
                    display.greetingResponse(
                        utterHandler.getResponse("invalid_month")
                    );
                    continue;
                }
                std::string m_str = userInput.substr(0, space);
                std::string y_str = userInput.substr(space + 1);
                std::string low_m = utterHandler.toLower(m_str);
                std::map<std::string, int> month_map = {
                    {"january", 1},
                    {"february", 2},
                    {"march", 3},
                    {"april", 4},
                    {"may", 5},
                    {"june", 6},
                    {"july", 7},
                    {"august", 8},
                    {"september", 9},
                    {"october", 10},
                    {"november", 11},
                    {"december", 12}
                };
                if (month_map.find(low_m) == month_map.end() ||
                    !is_digit(y_str)) {
                    display.greetingResponse(
                        utterHandler.getResponse("invalid_month")
                    );
                    continue;
                }
                std::vector<std::string> insts;
                std::vector<std::string> rems;
                std::string total;
                std::string down;
                personalSelection.calculateInstallmentBreakdown(
                    selected_personal,
                    insts,
                    rems,
                    total,
                    down
                );
                auto months = generate_months(userInput, insts.size());
                display.monthlyPlanDisplay(
                    total,
                    down,
                    months,
                    insts,
                    rems
                );
                std::string promptApply =
                    utterHandler.getResponse("prompt_apply");
                display.promptForInput(promptApply);
                currentState = State::PROCEED_APPLY;
            }
        } else if (currentState == State::PROCEED_APPLY) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "yes" || low == "y") {
                current_app = Application();
                current_app.loan_type = current_loan_type;
                current_app.sub_type = current_sub_type;
                current_app.plan_id = current_plan_id;
                current_app.app_id =
                    appHandler.generateNextId(); // Added app_id generation
                current_app.status = "C0"; // Added initial status
                display.greetingResponse(
                    "Starting new application with ID " + current_app.app_id
                );
                display.greetingResponse(
                    "Entering Personal Information section."
                ); // Added section print
                currentState = State::COLLECT_FULL_NAME;
                display.promptForInput(
                    utterHandler.getResponse("prompt_full_name")
                );
            } else if (low == "no" || low == "n") {
                currentState = State::NORMAL;
            } else {
                std::string invalid =
                    utterHandler.getResponse("invalid_yes_no");
                display.greetingResponse(invalid);
            }
        } else if (currentState == State::COLLECT_FULL_NAME) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid name.");
            } else {
                current_app.full_name = originalInput;
                currentState = State::COLLECT_FATHER_NAME;
                display.promptForInput(
                    utterHandler.getResponse("prompt_father_name")
                );
            }
        } else if (currentState == State::COLLECT_FATHER_NAME) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid name.");
            } else {
                current_app.father_name = originalInput;
                currentState = State::COLLECT_ADDRESS;
                display.promptForInput(
                    utterHandler.getResponse("prompt_address")
                );
            }
        } else if (currentState == State::COLLECT_ADDRESS) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid address.");
            } else {
                current_app.postal_address = originalInput;
                currentState = State::COLLECT_CONTACT;
                display.promptForInput(
                    utterHandler.getResponse("prompt_contact")
                );
            }
        } else if (currentState == State::COLLECT_CONTACT) {
            if (userInput.length() == 11 && is_digit(userInput)) {
                current_app.contact_number = userInput;
                currentState = State::COLLECT_EMAIL;
                display.promptForInput(
                    utterHandler.getResponse("prompt_email")
                );
            } else {
                display.greetingResponse(
                    "Invalid contact number. Must be 11 digits."
                );
            }
        } else if (currentState == State::COLLECT_EMAIL) {
            if (validate_email(userInput)) {
                current_app.email_address = userInput;
                currentState = State::COLLECT_CNIC;
                display.promptForInput(
                    utterHandler.getResponse("prompt_cnic")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_email")
                );
            }
        } else if (currentState == State::COLLECT_CNIC) {
            if (userInput.length() == 13 && is_digit(userInput)) {
                current_app.cnic_number = userInput;
                currentState = State::COLLECT_CNIC_EXPIRY;
                display.promptForInput(
                    utterHandler.getResponse("prompt_cnic_expiry")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_cnic")
                );
            }
        } else if (currentState == State::COLLECT_CNIC_EXPIRY) {
            if (validate_date(userInput)) {
                current_app.cnic_expiry = userInput;
                currentState = State::COLLECT_EMPLOYMENT;
                display.promptForInput(
                    utterHandler.getResponse("prompt_employment")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_date")
                );
            }
        } else if (currentState == State::COLLECT_EMPLOYMENT) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "self-employed" ||
                low == "salaried" ||
                low == "retired" ||
                low == "unemployed") {
                current_app.employment_status = low;
                currentState = State::COLLECT_MARITAL;
                display.promptForInput(
                    utterHandler.getResponse("prompt_marital")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_employment")
                );
            }
        } else if (currentState == State::COLLECT_MARITAL) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "single" ||
                low == "married" ||
                low == "divorced" ||
                low == "widowed") {
                current_app.marital_status = low;
                currentState = State::COLLECT_GENDER;
                display.promptForInput(
                    utterHandler.getResponse("prompt_gender")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_marital")
                );
            }
        } else if (currentState == State::COLLECT_GENDER) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "male" ||
                low == "female" ||
                low == "other") {
                current_app.gender = low;
                currentState = State::COLLECT_DEPENDENTS;
                display.promptForInput(
                    utterHandler.getResponse("prompt_dependents")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_gender")
                );
            }
        } else if (currentState == State::COLLECT_DEPENDENTS) {
            if (is_digit(userInput)) {
                current_app.num_dependents = userInput;
                // Save after personal section
                if (appHandler.getById(current_app.app_id)) {
                    appHandler.update(current_app);
                } else {
                    appHandler.add(current_app);
                }
                appHandler.save("data/applications.txt");
                current_app.status = "C1";
                display.greetingResponse(
                    utterHandler.getResponse("section_saved") +
                    " Personal Information complete."
                );
                display.greetingResponse(
                    "Entering Financial Information section."
                ); // Added section print
                currentState = State::COLLECT_ANNUAL_INCOME;
                display.promptForInput(
                    utterHandler.getResponse("prompt_annual_income")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_number")
                );
            }
        } else if (currentState == State::COLLECT_ANNUAL_INCOME) {
            if (is_digit(userInput)) {
                current_app.annual_income = userInput;
                currentState = State::COLLECT_AVG_ELEC;
                display.promptForInput(
                    utterHandler.getResponse("prompt_avg_elec")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_number")
                );
            }
        } else if (currentState == State::COLLECT_AVG_ELEC) {
            if (is_digit(userInput)) {
                current_app.avg_electricity = userInput;
                currentState = State::COLLECT_CURR_ELEC;
                display.promptForInput(
                    utterHandler.getResponse("prompt_curr_elec")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_number")
                );
            }
        } else if (currentState == State::COLLECT_CURR_ELEC) {
            if (is_digit(userInput)) {
                current_app.current_electricity = userInput;
                currentState = State::COLLECT_HAS_EXISTING;
                display.promptForInput(
                    utterHandler.getResponse("prompt_has_existing")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_number")
                );
            }
        } else if (currentState == State::COLLECT_HAS_EXISTING) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "yes" || low == "y") {
                currentState = State::COLLECT_NUM_LOANS;
                display.promptForInput(
                    utterHandler.getResponse("prompt_num_loans")
                );
            } else if (low == "no" || low == "n") {
                current_app.existing_loans = "no";
                // Save after financial if no loans
                if (appHandler.getById(current_app.app_id)) {
                    appHandler.update(current_app);
                } else {
                    appHandler.add(current_app);
                }
                appHandler.save("data/applications.txt");
                current_app.status = "C2";
                display.greetingResponse(
                    utterHandler.getResponse("section_saved") +
                    " Financial Information complete."
                );
                display.greetingResponse(
                    "Entering References section."
                ); // Added section print
                currentState = State::COLLECT_REF1_NAME;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref1_name")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_yes_no")
                );
            }
        } else if (currentState == State::COLLECT_NUM_LOANS) {
            if (is_digit(userInput)) {
                num_existing = std::stoi(userInput);
                if (num_existing > 0) {
                    current_loan_index = 1;
                    currentState = State::COLLECT_LOAN_ACTIVE;
                    std::string prompt =
                        utterHandler.getResponse("prompt_loan_active");
                    prompt = utterHandler.replacePlaceholder(
                        prompt,
                        "{num}",
                        std::to_string(current_loan_index)
                    );
                    display.promptForInput(prompt);
                } else {
                    current_app.existing_loans = "no";
                    // Save after financial
                    if (appHandler.getById(current_app.app_id)) {
                        appHandler.update(current_app);
                    } else {
                        appHandler.add(current_app);
                    }
                    appHandler.save("data/applications.txt");
                    current_app.status = "C2";
                    display.greetingResponse(
                        utterHandler.getResponse("section_saved") +
                        " Financial Information complete."
                    );
                    display.greetingResponse(
                        "Entering References section."
                    ); // Added section print
                    currentState = State::COLLECT_REF1_NAME;
                    display.promptForInput(
                        utterHandler.getResponse("prompt_ref1_name")
                    );
                }
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_number")
                );
            }
        } else if (currentState == State::COLLECT_LOAN_ACTIVE) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "yes" || low == "y") {
                current_loan_data["active"] = "yes";
                currentState = State::COLLECT_LOAN_TOTAL;
                std::string prompt =
                    utterHandler.getResponse("prompt_loan_total");
                prompt = utterHandler.replacePlaceholder(
                    prompt,
                    "{num}",
                    std::to_string(current_loan_index)
                );
                display.promptForInput(prompt);
            } else if (low == "no" || low == "n") {
                current_loan_data["active"] = "no";
                currentState = State::COLLECT_LOAN_TOTAL;
                std::string prompt =
                    utterHandler.getResponse("prompt_loan_total");
                prompt = utterHandler.replacePlaceholder(
                    prompt,
                    "{num}",
                    std::to_string(current_loan_index)
                );
                display.promptForInput(prompt);
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_yes_no")
                );
            }
        } else if (currentState == State::COLLECT_LOAN_TOTAL) {
            if (is_digit(userInput)) {
                current_loan_data["total"] = userInput;
                currentState = State::COLLECT_LOAN_RETURNED;
                std::string prompt =
                    utterHandler.getResponse("prompt_loan_returned");
                prompt = utterHandler.replacePlaceholder(
                    prompt,
                    "{num}",
                    std::to_string(current_loan_index)
                );
                display.promptForInput(prompt);
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_number")
                );
            }
        } else if (currentState == State::COLLECT_LOAN_RETURNED) {
            if (is_digit(userInput)) {
                current_loan_data["returned"] = userInput;
                currentState = State::COLLECT_LOAN_DUE;
                std::string prompt =
                    utterHandler.getResponse("prompt_loan_due");
                prompt = utterHandler.replacePlaceholder(
                    prompt,
                    "{num}",
                    std::to_string(current_loan_index)
                );
                display.promptForInput(prompt);
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_number")
                );
            }
        } else if (currentState == State::COLLECT_LOAN_DUE) {
            if (is_digit(userInput)) {
                current_loan_data["due"] = userInput;
                currentState = State::COLLECT_LOAN_BANK;
                std::string prompt =
                    utterHandler.getResponse("prompt_loan_bank");
                prompt = utterHandler.replacePlaceholder(
                    prompt,
                    "{num}",
                    std::to_string(current_loan_index)
                );
                display.promptForInput(prompt);
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_number")
                );
            }
        } else if (currentState == State::COLLECT_LOAN_BANK) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid bank name.");
            } else {
                current_loan_data["bank"] = originalInput;
                currentState = State::COLLECT_LOAN_CATEGORY;
                std::string prompt =
                    utterHandler.getResponse("prompt_loan_category");
                prompt = utterHandler.replacePlaceholder(
                    prompt,
                    "{num}",
                    std::to_string(current_loan_index)
                );
                display.promptForInput(prompt);
            }
        } else if (currentState == State::COLLECT_LOAN_CATEGORY) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "car" ||
                low == "home" ||
                low == "bike" ||
                low == "scooter" ||
                low == "personal") {
                current_loan_data["category"] = low;
                existing_loans_vec.push_back(current_loan_data);
                current_loan_data.clear();
                ++current_loan_index;
                if (current_loan_index <= num_existing) {
                    currentState = State::COLLECT_LOAN_ACTIVE;
                    std::string prompt =
                        utterHandler.getResponse("prompt_loan_active");
                    prompt = utterHandler.replacePlaceholder(
                        prompt,
                        "{num}",
                        std::to_string(current_loan_index)
                    );
                    display.promptForInput(prompt);
                } else {
                    current_app.existing_loans = "yes";
                    for (const auto &m : existing_loans_vec) {
                        current_app.existing_loans +=
                            "|" +
                            m.at("active") + "," +
                            m.at("total") + "," +
                            m.at("returned") + "," +
                            m.at("due") + "," +
                            m.at("bank") + "," +
                            m.at("category");
                    }
                    existing_loans_vec.clear();
                    num_existing = 0;
                    current_loan_index = 0;
                    // Save after financial with loans
                    if (appHandler.getById(current_app.app_id)) {
                        appHandler.update(current_app);
                    } else {
                        appHandler.add(current_app);
                    }
                    appHandler.save("data/applications.txt");
                    current_app.status = "C2";
                    display.greetingResponse(
                        utterHandler.getResponse("section_saved") +
                        " Financial Information complete."
                    );
                    display.greetingResponse(
                        "Entering References section."
                    ); // Added section print
                    currentState = State::COLLECT_REF1_NAME;
                    display.promptForInput(
                        utterHandler.getResponse("prompt_ref1_name")
                    );
                }
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_category")
                );
            }
        } else if (currentState == State::COLLECT_REF1_NAME) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid name.");
            } else {
                current_app.ref1_name = originalInput;
                currentState = State::COLLECT_REF1_CNIC;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref1_cnic")
                );
            }
        } else if (currentState == State::COLLECT_REF1_CNIC) {
            if (userInput.length() == 13 && is_digit(userInput)) {
                current_app.ref1_cnic = userInput;
                currentState = State::COLLECT_REF1_ISSUE;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref1_issue")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_cnic")
                );
            }
        } else if (currentState == State::COLLECT_REF1_ISSUE) {
            if (validate_date(userInput)) {
                current_app.ref1_issue = userInput;
                currentState = State::COLLECT_REF1_PHONE;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref1_phone")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_date")
                );
            }
        } else if (currentState == State::COLLECT_REF1_PHONE) {
            if (userInput.length() == 11 && is_digit(userInput)) {
                current_app.ref1_phone = userInput;
                currentState = State::COLLECT_REF1_EMAIL;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref1_email")
                );
            } else {
                display.greetingResponse(
                    "Invalid phone number. Must be 11 digits."
                );
            }
        } else if (currentState == State::COLLECT_REF1_EMAIL) {
            if (validate_email(userInput)) {
                current_app.ref1_email = userInput;
                currentState = State::COLLECT_REF2_NAME;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref2_name")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_email")
                );
            }
        } else if (currentState == State::COLLECT_REF2_NAME) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid name.");
            } else {
                current_app.ref2_name = originalInput;
                currentState = State::COLLECT_REF2_CNIC;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref2_cnic")
                );
            }
        } else if (currentState == State::COLLECT_REF2_CNIC) {
            if (userInput.length() == 13 && is_digit(userInput)) {
                current_app.ref2_cnic = userInput;
                currentState = State::COLLECT_REF2_ISSUE;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref2_issue")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_cnic")
                );
            }
        } else if (currentState == State::COLLECT_REF2_ISSUE) {
            if (validate_date(userInput)) {
                current_app.ref2_issue = userInput;
                currentState = State::COLLECT_REF2_PHONE;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref2_phone")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_date")
                );
            }
        } else if (currentState == State::COLLECT_REF2_PHONE) {
            if (userInput.length() == 11 && is_digit(userInput)) {
                current_app.ref2_phone = userInput;
                currentState = State::COLLECT_REF2_EMAIL;
                display.promptForInput(
                    utterHandler.getResponse("prompt_ref2_email")
                );
            } else {
                display.greetingResponse(
                    "Invalid phone number. Must be 11 digits."
                );
            }
        } else if (currentState == State::COLLECT_REF2_EMAIL) {
            if (validate_email(userInput)) {
                current_app.ref2_email = userInput;
                // Save after references
                if (appHandler.getById(current_app.app_id)) {
                    appHandler.update(current_app);
                } else {
                    appHandler.add(current_app);
                }
                appHandler.save("data/applications.txt");
                current_app.status = "C3";
                display.greetingResponse(
                    utterHandler.getResponse("section_saved") +
                    " References complete."
                );
                display.greetingResponse(
                    "Entering Documents section."
                ); // Added section print
                currentState = State::COLLECT_CNIC_FRONT;
                display.promptForInput(
                    utterHandler.getResponse("prompt_cnic_front")
                );
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_email")
                );
            }
        } else if (currentState == State::COLLECT_CNIC_FRONT) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid path.");
            } else {
                current_app.cnic_front = userInput;
                currentState = State::COLLECT_CNIC_BACK;
                display.promptForInput(
                    utterHandler.getResponse("prompt_cnic_back")
                );
            }
        } else if (currentState == State::COLLECT_CNIC_BACK) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid path.");
            } else {
                current_app.cnic_back = userInput;
                currentState = State::COLLECT_ELEC_BILL;
                display.promptForInput(
                    utterHandler.getResponse("prompt_elec_bill")
                );
            }
        } else if (currentState == State::COLLECT_ELEC_BILL) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid path.");
            } else {
                current_app.elec_bill = userInput;
                currentState = State::COLLECT_SALARY_SLIP;
                display.promptForInput(
                    utterHandler.getResponse("prompt_salary_slip")
                );
            }
        } else if (currentState == State::COLLECT_SALARY_SLIP) {
            if (userInput.empty()) {
                display.greetingResponse("Please enter a valid path.");
            } else {
                current_app.salary_slip = userInput;
                currentState = State::COLLECT_CONFIRM;
                display.promptForInput(
                    utterHandler.getResponse("prompt_confirm")
                );
            }
        } else if (currentState == State::COLLECT_CONFIRM) {
            std::string low = utterHandler.toLower(userInput);
            if (low == "yes" || low == "y") {
                current_app.status = "submitted"; // Set to submitted
                if (appHandler.getById(current_app.app_id)) {
                    appHandler.update(current_app);
                } else {
                    current_app.app_id = appHandler.generateNextId();
                    appHandler.add(current_app);
                }
                appHandler.save("data/applications.txt");
                std::string success =
                    utterHandler.getResponse("submit_success");
                success = utterHandler.replacePlaceholder(
                    success,
                    "{id}",
                    current_app.app_id
                );
                display.greetingResponse(success);
                currentState = State::NORMAL;
            } else if (low == "no" || low == "n") {
                save_partial();
                currentState = State::NORMAL;
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_yes_no")
                );
            }
        } else if (currentState == State::COUNT_CNIC) {
            if (userInput.length() == 13 && is_digit(userInput)) {
                int sub = appHandler.countSubmitted(userInput);
                int appr = appHandler.countApproved(userInput);
                int rej = appHandler.countRejected(userInput);
                std::string result =
                    utterHandler.getResponse("count_result");
                result = utterHandler.replacePlaceholder(
                    result,
                    "{submitted}",
                    std::to_string(sub)
                );
                result = utterHandler.replacePlaceholder(
                    result,
                    "{approved}",
                    std::to_string(appr)
                );
                result = utterHandler.replacePlaceholder(
                    result,
                    "{rejected}",
                    std::to_string(rej)
                );
                display.greetingResponse(result);
                currentState = State::NORMAL;
            } else {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_cnic")
                );
            }
        } else if (currentState == State::VIEW_PLAN_ID) {
            current_app_id = userInput;
            auto app = appHandler.getById(userInput);
            if (!app) {
                display.undefinedInputResponse("Invalid application ID.");
                currentState = State::NORMAL;
            } else if (app->status != "approved") {
                display.undefinedInputResponse(
                    utterHandler.getResponse("not_approved")
                );
                currentState = State::NORMAL;
            } else if (app->starting_month.empty()) {
                currentState = State::SET_START_MONTH;
                display.promptForInput(
                    utterHandler.getResponse("start_month_prompt")
                );
            } else {
                generate_plan(*app);
                currentState = State::NORMAL;
            }
        } else if (currentState == State::SET_START_MONTH) {
            // Validate
            size_t space = userInput.find(' ');
            if (space == std::string::npos) {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_month")
                );
                continue;
            }
            std::string m_str = userInput.substr(0, space);
            std::string y_str = userInput.substr(space + 1);
            std::string low_m = utterHandler.toLower(m_str);
            std::map<std::string, int> month_map = {
                {"january", 1},
                {"february", 2},
                {"march", 3},
                {"april", 4},
                {"may", 5},
                {"june", 6},
                {"july", 7},
                {"august", 8},
                {"september", 9},
                {"october", 10},
                {"november", 11},
                {"december", 12}
            };
            if (month_map.find(low_m) == month_map.end() ||
                !is_digit(y_str)) {
                display.greetingResponse(
                    utterHandler.getResponse("invalid_month")
                );
                continue;
            }
            auto app = appHandler.getById(current_app_id);
            if (app) {
                app->starting_month = userInput;
                appHandler.update(*app);
                appHandler.save("data/applications.txt");
                generate_plan(*app);
            }
            currentState = State::NORMAL;
        } else if (currentState == State::RESUME_ID) { // Added for resume ID
            resume_id = userInput;
            currentState = State::RESUME_CNIC;
            display.promptForInput(
                utterHandler.getResponse("prompt_resume_cnic")
            );
        } else if (currentState == State::RESUME_CNIC) { // Added for resume CNIC verify
            resume_cnic = userInput;
            auto app = appHandler.getById(resume_id);
            if (!app ||
                app->cnic_number != resume_cnic ||
                !appHandler.isIncomplete(app->status)) {
                display.undefinedInputResponse(
                    "Invalid ID or CNIC, or application not resumable."
                );
                currentState = State::NORMAL;
            } else {
                current_app = *app;
                current_loan_type = app->loan_type;
                current_sub_type = app->sub_type;
                current_plan_id = app->plan_id;
                if (app->status == "C1") {
                    display.greetingResponse(
                        "Entering Financial Information section."
                    );
                    currentState = State::COLLECT_ANNUAL_INCOME;
                    display.promptForInput(
                        utterHandler.getResponse("prompt_annual_income")
                    );
                } else if (app->status == "C2") {
                    display.greetingResponse(
                        "Entering References section."
                    );
                    currentState = State::COLLECT_REF1_NAME;
                    display.promptForInput(
                        utterHandler.getResponse("prompt_ref1_name")
                    );
                } else if (app->status == "C3") {
                    display.greetingResponse(
                        "Entering Documents section."
                    );
                    currentState = State::COLLECT_CNIC_FRONT;
                    display.promptForInput(
                        utterHandler.getResponse("prompt_cnic_front")
                    );
                }
                display.greetingResponse("Resuming application " + resume_id);
            }
        } else if (currentState == State::GENERAL) { // Added for general chat
            if (userInput == "back" || userInput == "exit") {
                display.greetingResponse(
                    utterHandler.getResponse("exit_general")
                );
                currentState = State::NORMAL;
            } else {
                std::string resp = chatHandler.getResponse(originalInput);
                display.greetingResponse(resp);
            }
        } else if (userInput == "pause" &&
                   currentState >= State::PROCEED_APPLY &&
                   currentState <= State::COLLECT_CONFIRM) { // Added pause handling in app states
            save_partial();
            currentState = State::NORMAL;
        }
    }
    return 0;
}
