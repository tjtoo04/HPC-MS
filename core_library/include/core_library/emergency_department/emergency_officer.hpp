#ifndef EMERGENCY_OFFICER_HPP
#define EMERGENCY_OFFICER_HPP

#include "core_library/emergency_department/priority_queue.hpp"
#include "core_library/emergency_department/emergency_case.hpp"
#include <string>

// ANSI color codes
#define C_RESET "\033[0m"
#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_BLUE "\033[34m"
#define C_MAGENTA "\033[35m"
#define C_CYAN "\033[36m"
#define C_BOLD "\033[1m"

class EmergencyOfficer
{
private:
    PriorityQueue emergencyQueue;
    int nextCaseNumber;  // Auto-increment for case IDs
    
    // Helper functions
    std::string generateNextCaseID();
    void displayTriageInfo() const;
    void displayEmergencyTypeInfo() const;
    TriageLevel selectTriageLevel() const;
    EmergencyType selectEmergencyType() const;
    void displayStatistics() const;
    void displayCaseHistory() const;
    
    // File I/O
    void loadCasesFromFile(const std::string& filename);
    void saveCasesToFile(const std::string& filename) const;
    
public:
    // Constructor
    EmergencyOfficer();
    
    // Destructor
    ~EmergencyOfficer();
    
    // ========== REQUIRED FUNCTIONALITIES ==========
    
    // Functionality 1: Log Emergency Case
    void logEmergencyCase();
    
    // Functionality 2: Process Most Critical Case
    void processMostCriticalCase();
    
    // Functionality 3: View Pending Emergency Cases
    void viewPendingCases();
    
    // ========== ADDITIONAL FEATURES (High Distinction) ==========
    
    // Feature 1: Automatic Priority Escalation
    // Cases waiting too long get priority boost
    void checkPriorityEscalation();
    
    // Feature 2: Search Case by ID or Name
    void searchCase();
    
    // Feature 3: Display Statistics Dashboard
    void displayDashboard();
    
    // Feature 4: Triage Guidelines Reference
    void showTriageGuidelines();
    
    // Feature 5: Batch Case Processing
    // Process multiple cases at once (e.g., end of shift)
    void batchProcessCases();
    
    // Feature 6: Export Cases to Report
    void exportCasesToFile();
    
    // Menu and main loop
    void displayMenu();
    void run();
};

#endif