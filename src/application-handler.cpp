// File: src/application-handler.cpp (Unchanged - But now works with # delimiter)
#include "../include/application-handler.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cctype>

ApplicationHandler::ApplicationHandler() {}

bool ApplicationHandler::load(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) return true; // If no file, empty is ok

    applications.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Application app;
        app.deserialize(line);
        applications.push_back(app);
    }
    file.close();
    return true;
}

bool ApplicationHandler::save(const std::string &file_name) const {
    std::ofstream file(file_name);
    if (!file.is_open()) return false;

    for (const auto& app : applications) {
        file << app.serialize() << "\n";
    }
    file.close();
    return true;
}

std::string ApplicationHandler::generateNextId() const {
    if (applications.empty()) return "0001";
    int max_id = 0;
    for (const auto& app : applications) {
        try {
            int id = std::stoi(app.app_id);
            if (id > max_id) max_id = id;
        } catch (...) {}
    }
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << (max_id + 1);
    return ss.str();
}

void ApplicationHandler::add(const Application& app) {
    applications.push_back(app);
}

int ApplicationHandler::countSubmitted(const std::string& cnic) const {
    int count = 0;
    for (const auto& app : applications) {
        if (app.cnic_number == cnic && app.status == "submitted") ++count;
    }
    return count;
}

int ApplicationHandler::countApproved(const std::string& cnic) const {
    int count = 0;
    for (const auto& app : applications) {
        if (app.cnic_number == cnic && app.status == "approved") ++count;
    }
    return count;
}

int ApplicationHandler::countRejected(const std::string& cnic) const {
    int count = 0;
    for (const auto& app : applications) {
        if (app.cnic_number == cnic && app.status == "rejected") ++count;
    }
    return count;
}

Application* ApplicationHandler::getById(const std::string& id) {
    for (auto& app : applications) {
        if (app.app_id == id) return &app;
    }
    return nullptr;
}

const Application* ApplicationHandler::getById(const std::string& id) const {
    for (const auto& app : applications) {
        if (app.app_id == id) return &app;
    }
    return nullptr;
}

void ApplicationHandler::update(const Application& updated_app) {
    for (auto& app : applications) {
        if (app.app_id == updated_app.app_id) {
            app = updated_app;
            break;
        }
    }
}

bool ApplicationHandler::isIncomplete(const std::string& status) const {
    return status == "C1" || status == "C2" || status == "C3";
}
