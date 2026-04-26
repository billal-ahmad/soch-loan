#include "../include/utterance-handler.h"
#include "../include/display.h"  // add this include for the display class
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

// File handling to trim spaces
std::string UtteranceHandler::trim(const std::string &str) const {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

// Converting to lower case to make input case-insensitive
std::string UtteranceHandler::toLower(const std::string &str) const {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

// Default constructor
UtteranceHandler::UtteranceHandler() {}

// Parameterized constructor
UtteranceHandler::UtteranceHandler(const std::string &file_name) {
    storeUserInputAndResponse(file_name);
}

bool UtteranceHandler::storeUserInputAndResponse(const std::string &file_name) {
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file!! " << file_name << "\n";
        return false;
    }

    _input.clear();
    _response.clear();
    std::string line;

    while (std::getline(file, line)) {
        // Trim the line first to handle spaces at beginning/end
        std::string trimmedLine = trim(line);
        if (trimmedLine.empty()) continue;

        size_t pos = trimmedLine.find('#');
        if (pos == std::string::npos) continue;

        std::string key = trim(trimmedLine.substr(0, pos));
        std::string value = trim(trimmedLine.substr(pos + 1));

        // Converting to lower case for case-insensitive matching
        key = toLower(key);

        _input.push_back(key);
        _response.push_back(value);
    }

    file.close();
    return true;
}

std::string UtteranceHandler::generateResponse(const std::string &input) {
    // Properly handle multiple spaces by trimming and normalizing
    std::string cleanedInput = trim(input);
    cleanedInput = toLower(cleanedInput);

    // Search for exact match first
    for (size_t i = 0; i < _input.size(); i++) {
        if (_input[i] == cleanedInput) return _response[i];
    }

    // If no exact match found, return empty
    return "";
}

std::string UtteranceHandler::getResponse(const std::string &key) const {
    std::string cleanedKey = toLower(trim(key));

    for (size_t i = 0; i < _input.size(); i++) {
        if (_input[i] == cleanedKey) return _response[i];
    }

    return "";
}

std::string UtteranceHandler::replacePlaceholder(const std::string &text,
                                                 const std::string &placeholder,
                                                 const std::string &value) const {
    std::string res = text;
    size_t pos = res.find(placeholder);

    while (pos != std::string::npos) {
        res.replace(pos, placeholder.length(), value);
        pos = res.find(placeholder, pos + value.length());
    }

    return res;
}
