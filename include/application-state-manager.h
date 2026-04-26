#pragma once
#include <string>
#include <vector>
#include <map>
#include "application.h"

/**
 * ApplicationStateManager - Manages the state machine for loan application workflow
 * 
 * This class encapsulates all state transitions and handles the application flow,
 * improving separation of concerns from the main loop logic.
 */
class ApplicationStateManager {
public:
    // Application workflow states
    enum class State {
        NORMAL,
        SELECT_LOAN_TYPE,
        ASK_RESUME,
        SELECT_AREA_HOME,
        SELECT_PLAN_HOME,
        CONFIRM_INSTALLMENT_HOME,
        SELECT_MAKE_CAR,
        SELECT_PLAN_CAR,
        CONFIRM_INSTALLMENT_CAR,
        SELECT_MAKE_SCOOTER,
        SELECT_PLAN_SCOOTER,
        CONFIRM_INSTALLMENT_SCOOTER,
        SELECT_CATEGORY_PERSONAL,
        SELECT_PLAN_PERSONAL,
        CONFIRM_INSTALLMENT_PERSONAL,
        PROCEED_APPLY,
        COLLECT_PERSONAL_INFO,      // Group personal collection
        COLLECT_EMPLOYMENT_INFO,    // Group employment info
        COLLECT_EXISTING_LOANS,     // Group existing loans
        COLLECT_REFERENCES,         // Group references
        COLLECT_DOCUMENTS,          // Group documents
        COLLECT_CONFIRM,
        RESUME_ID,
        RESUME_CNIC,
        VIEW_PLAN_ID,
        SET_START_MONTH,
        COUNT_CNIC,
        GENERAL
    };

    ApplicationStateManager();

    // State management
    State getCurrentState() const;
    void setCurrentState(State newState);
    bool canTransitionTo(State nextState) const;
    
    // State grouping helpers
    bool isInPersonalInfoCollection() const;
    bool isInEmploymentInfoCollection() const;
    bool isInExistingLoansCollection() const;
    bool isInReferencesCollection() const;
    bool isInDocumentsCollection() const;
    bool isInLoanSelection() const;
    
    // Progress tracking for multi-step sections
    int getProgressInSection() const;
    void advanceProgressInSection();
    void resetSectionProgress();

private:
    State _currentState;
    int _sectionProgress;
};
