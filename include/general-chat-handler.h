#pragma once
#include <vector>
#include <string>
#include <set>
#include <map>

class GeneralChatHandler {
private:
    std::vector<std::pair<std::string, std::string>> qas;
    std::string trim(const std::string &str) const;
    std::string toLower(const std::string &str) const;
public:
    GeneralChatHandler();
    GeneralChatHandler(const std::string &file_name);
    bool load(const std::string &file_name);
    std::string getResponse(const std::string &input) const;
};
