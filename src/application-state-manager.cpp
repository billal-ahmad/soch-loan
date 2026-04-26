#include "application-state-manager.h"
#include <algorithm>

ApplicationStateManager::ApplicationStateManager() 
    : _currentState(State::NORMAL), _sectionProgress(0) {}

ApplicationStateManager::State ApplicationStateManager::getCurrentState() const {
    return _currentState;
}

void ApplicationStateManager::setCurrentState(State newState) {
    _currentState = newState;
}

bool ApplicationStateManager::canTransitionTo(State nextState) const {
    // Define valid state transitions
    // This prevents invalid state sequences
    std::map<State, std::vector<State>> validTransitions = {
        {State::NORMAL, {State::SELECT_LOAN_TYPE, State::GENERAL}},
        {State::SELECT_LOAN_TYPE, {State::ASK_RESUME, State::NORMAL}},
        {State::ASK_RESUME, {State::PROCEED_APPLY, State::RESUME_ID, State::NORMAL}},
        {State::SELECT_AREA_HOME, {State::SELECT_PLAN_HOME, State::NORMAL}},
        {State::SELECT_PLAN_HOME, {State::CONFIRM_INSTALLMENT_HOME, State::NORMAL}},
        {State::CONFIRM_INSTALLMENT_HOME, {State::PROCEED_APPLY, State::NORMAL}},
        {State::PROCEED_APPLY, {State::COLLECT_PERSONAL_INFO, State::NORMAL}},
        {State::COLLECT_PERSONAL_INFO, {State::COLLECT_EMPLOYMENT_INFO, State::NORMAL}},
        {State::COLLECT_EMPLOYMENT_INFO, {State::COLLECT_EXISTING_LOANS, State::NORMAL}},
        {State::COLLECT_EXISTING_LOANS, {State::COLLECT_REFERENCES, State::NORMAL}},
        {State::COLLECT_REFERENCES, {State::COLLECT_DOCUMENTS, State::NORMAL}},
        {State::COLLECT_DOCUMENTS, {State::COLLECT_CONFIRM, State::NORMAL}},
        {State::COLLECT_CONFIRM, {State::NORMAL}},
    };

    auto it = validTransitions.find(_currentState);
    if (it != validTransitions.end()) {
        const auto& validStates = it->second;
        return std::find(validStates.begin(), validStates.end(), nextState) != validStates.end();
    }
    return false;
}

bool ApplicationStateManager::isInPersonalInfoCollection() const {
    return _currentState == State::COLLECT_PERSONAL_INFO;
}

bool ApplicationStateManager::isInEmploymentInfoCollection() const {
    return _currentState == State::COLLECT_EMPLOYMENT_INFO;
}

bool ApplicationStateManager::isInExistingLoansCollection() const {
    return _currentState == State::COLLECT_EXISTING_LOANS;
}

bool ApplicationStateManager::isInReferencesCollection() const {
    return _currentState == State::COLLECT_REFERENCES;
}

bool ApplicationStateManager::isInDocumentsCollection() const {
    return _currentState == State::COLLECT_DOCUMENTS;
}

bool ApplicationStateManager::isInLoanSelection() const {
    return _currentState == State::SELECT_AREA_HOME ||
           _currentState == State::SELECT_PLAN_HOME ||
           _currentState == State::SELECT_MAKE_CAR ||
           _currentState == State::SELECT_PLAN_CAR ||
           _currentState == State::SELECT_MAKE_SCOOTER ||
           _currentState == State::SELECT_PLAN_SCOOTER ||
           _currentState == State::SELECT_CATEGORY_PERSONAL ||
           _currentState == State::SELECT_PLAN_PERSONAL;
}

int ApplicationStateManager::getProgressInSection() const {
    return _sectionProgress;
}

void ApplicationStateManager::advanceProgressInSection() {
    ++_sectionProgress;
}

void ApplicationStateManager::resetSectionProgress() {
    _sectionProgress = 0;
}
