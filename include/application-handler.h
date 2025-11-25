#pragma once
#include "application.h"
#include <vector>
#include <string>

class ApplicationHandler {
private:
    std::vector<Application> applications;
public:
    const std::vector<Application>& getApplications() const { return applications; }
    ApplicationHandler();
    bool load(const std::string& file_name);
    bool save(const std::string& file_name) const;
    std::string generateNextId() const;
    void add(const Application& app);
    int countSubmitted(const std::string& cnic) const;
    int countApproved(const std::string& cnic) const;
    int countRejected(const std::string& cnic) const;
    Application* getById(const std::string& id);
    const Application* getById(const std::string& id) const;
    void update(const Application& app);
    bool isIncomplete(const std::string& status) const;
};
