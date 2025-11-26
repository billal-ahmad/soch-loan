#include "../include/application.h"
#include <sstream>
#include <iostream>
#include <vector>

Application::Application() : status("submitted") {}

std::string Application::serialize() const {
    std::stringstream ss;
    ss << app_id
       << "#" << loan_type
       << "#" << sub_type
       << "#" << plan_id
       << "#" << full_name
       << "#" << father_name
       << "#" << postal_address
       << "#" << contact_number
       << "#" << email_address
       << "#" << cnic_number
       << "#" << cnic_expiry
       << "#" << employment_status
       << "#" << marital_status
       << "#" << gender
       << "#" << num_dependents
       << "#" << annual_income
       << "#" << avg_electricity
       << "#" << current_electricity
       << "#" << existing_loans
       << "#" << ref1_name
       << "#" << ref1_cnic
       << "#" << ref1_issue
       << "#" << ref1_phone
       << "#" << ref1_email
       << "#" << ref2_name
       << "#" << ref2_cnic
       << "#" << ref2_issue
       << "#" << ref2_phone
       << "#" << ref2_email
       << "#" << cnic_front
       << "#" << cnic_back
       << "#" << elec_bill
       << "#" << salary_slip
       << "#" << status
       << "#" << starting_month;
    return ss.str();
}

void Application::deserialize(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> parts;
    while (std::getline(ss, token, '#')) {
        parts.push_back(token);
    }
    size_t num = parts.size();
    if (num >= 1) app_id = parts[0];
    if (num >= 2) loan_type = parts[1];
    if (num >= 3) sub_type = parts[2];
    if (num >= 4) plan_id = parts[3];
    if (num >= 5) full_name = parts[4];
    if (num >= 6) father_name = parts[5];
    if (num >= 7) postal_address = parts[6];
    if (num >= 8) contact_number = parts[7];
    if (num >= 9) email_address = parts[8];
    if (num >= 10) cnic_number = parts[9];
    if (num >= 11) cnic_expiry = parts[10];
    if (num >= 12) employment_status = parts[11];
    if (num >= 13) marital_status = parts[12];
    if (num >= 14) gender = parts[13];
    if (num >= 15) num_dependents = parts[14];
    if (num >= 16) annual_income = parts[15];
    if (num >= 17) avg_electricity = parts[16];
    if (num >= 18) current_electricity = parts[17];
    if (num >= 19) existing_loans = parts[18];
    if (num >= 20) ref1_name = parts[19];
    if (num >= 21) ref1_cnic = parts[20];
    if (num >= 22) ref1_issue = parts[21];
    if (num >= 23) ref1_phone = parts[22];
    if (num >= 24) ref1_email = parts[23];
    if (num >= 25) ref2_name = parts[24];
    if (num >= 26) ref2_cnic = parts[25];
    if (num >= 27) ref2_issue = parts[26];
    if (num >= 28) ref2_phone = parts[27];
    if (num >= 29) ref2_email = parts[28];
    if (num >= 30) cnic_front = parts[29];
    if (num >= 31) cnic_back = parts[30];
    if (num >= 32) elec_bill = parts[31];
    if (num >= 33) salary_slip = parts[32];
    if (num >= 34) status = parts[33];
    if (num >= 35) starting_month = parts[34];
    else starting_month = "";
}
