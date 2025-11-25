// src/general-chat-handler.cpp
#include "../include/general-chat-handler.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

std::string GeneralChatHandler::trim(const std::string &str) const {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::string GeneralChatHandler::toLower(const std::string &str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

GeneralChatHandler::GeneralChatHandler() {}

GeneralChatHandler::GeneralChatHandler(const std::string &file_name) {
    load(file_name);
}

bool GeneralChatHandler::load(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << file_name << std::endl;
        return false;
    }

    qas.clear();
    std::string line;
    std::string q, a;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.find("Human 1:") == 0) {
            q = trim(line.substr(8));
            q = toLower(q);
        } else if (line.find("Human 2:") == 0 && !q.empty()) {
            a = trim(line.substr(8));
            qas.push_back({q, a});
            q.clear();
        }
    }
    file.close();
    return true;
}

std::string GeneralChatHandler::getResponse(const std::string &input) const {
    std::string cleaned = toLower(trim(input));
    if (cleaned.empty()) return "Sorry, I didn't understand that.";

    std::set<std::string> in_tokens;
    std::stringstream ss(cleaned);
    std::string tok;
    while (ss >> tok) {
        in_tokens.insert(tok);
    }

    double max_iou = -1.0;
    std::string best_response = "Sorry, I didn't understand that.";

    for (const auto& qa : qas) {
        std::set<std::string> q_tokens;
        std::stringstream qss(qa.first);
        while (qss >> tok) {
            q_tokens.insert(tok);
        }

        std::set<std::string> intersection, union_set;
        std::set_intersection(in_tokens.begin(), in_tokens.end(), q_tokens.begin(), q_tokens.end(), std::inserter(intersection, intersection.begin()));
        std::set_union(in_tokens.begin(), in_tokens.end(), q_tokens.begin(), q_tokens.end(), std::inserter(union_set, union_set.begin()));

        double iou = union_set.empty() ? 0.0 : static_cast<double>(intersection.size()) / static_cast<double>(union_set.size());

        if (iou > max_iou) {
            max_iou = iou;
            best_response = qa.second;
        }
    }

    return best_response;
}
