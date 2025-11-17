#include "core_library/emergency_department/emergency_case.hpp"
#include <sstream>
#include <ctime>
#include <iomanip>

// ANSI Color codes
#define C_RESET "\033[0m"
#define C_RED "\033[31m"
#define C_ORANGE "\033[38;5;208m"
#define C_YELLOW "\033[33m"
#define C_GREEN "\033[32m"
#define C_CYAN "\033[36m"
#define C_BOLD "\033[1m"

// Default constructor
EmergencyCase::EmergencyCase()
    : caseID(""), patientName(""), emergencyType(TYPE_C),
      triageLevel(NON_URGENT), symptoms(""), 
      arrivalTime(""), arrivalDate(""), waitingMinutes(0) {}

// Parameterized constructor
EmergencyCase::EmergencyCase(std::string id, std::string name, 
                             EmergencyType type, TriageLevel triage, 
                             std::string symptom)
    : caseID(id), patientName(name), emergencyType(type),
      triageLevel(triage), symptoms(symptom), waitingMinutes(0)
{
    arrivalTime = getCurrentTimeString();
    arrivalDate = getCurrentDateString();
}

// Display case information with beautiful formatting
void EmergencyCase::display() const
{
    std::string color = getTriageColor();
    std::string triageName = getTriageLevelName();
    std::string typeName = getEmergencyTypeName();
    
    std::cout << "╔═══════════════════════════════════════════════════════╗\n";
    std::cout << "║ " << C_BOLD << "Case ID: " << C_RESET << std::left 
              << std::setw(43) << caseID <<                  "║\n";
    std::cout << "║ " << C_BOLD << "Patient: " << C_RESET << std::left 
              << std::setw(43) << patientName <<             "║\n";
    std::cout << "║ " << C_BOLD << "Triage: " << C_RESET << color << std::left 
              << std::setw(43) << triageName << C_RESET <<   "║\n";
    std::cout << "║ " << C_BOLD << "Type: " << C_RESET << std::left 
              << std::setw(46) << typeName <<                "║\n";
    std::cout << "║ " << C_BOLD << "Symptoms: " << C_RESET << std::left 
              << std::setw(42) << symptoms <<                "║\n";
    std::cout << "║ " << C_BOLD << "Arrival: " << C_RESET << std::left 
              << std::setw(43) << (arrivalDate + " " + arrivalTime) <<                                             "║\n";
    std::cout << "║ " << C_BOLD << "Waiting: " << C_RESET;
    
    if (waitingMinutes > 60)
        std::cout << C_RED;
    else if (waitingMinutes > 30)
        std::cout << C_YELLOW;
    else
        std::cout << C_GREEN;
    
    std::cout << std::left << std::setw(43) << (std::to_string(waitingMinutes) + " minutes") 
              << C_RESET << "║\n";
    std::cout << "╚═══════════════════════════════════════════════════════╝\n";
}

// Calculate priority score (lower = higher priority)
// Formula: (triage_level * 100) - (waiting_minutes)
// This allows priority escalation for cases waiting too long
int EmergencyCase::getPriorityScore() const
{
    int baseScore = static_cast<int>(triageLevel) * 100;
    
    // Subtract waiting time to escalate priority
    // Each minute of waiting reduces score by 1
    int escalationBonus = waitingMinutes;
    
    return baseScore - escalationBonus;
}

// Convert to string for file storage (CSV format)
std::string EmergencyCase::toString() const
{
    std::stringstream ss;
    ss << caseID << ","
       << patientName << ","
       << static_cast<int>(emergencyType) << ","
       << static_cast<int>(triageLevel) << ","
       << symptoms << ","
       << arrivalTime << ","
       << arrivalDate << ","
       << waitingMinutes;
    return ss.str();
}

// Parse from file string
EmergencyCase EmergencyCase::fromString(const std::string& line)
{
    EmergencyCase ec;
    std::stringstream ss(line);
    std::string token;
    int field = 0;
    
    while (std::getline(ss, token, ','))
    {
        switch(field)
        {
            case 0: ec.caseID = token; break;
            case 1: ec.patientName = token; break;
            case 2: ec.emergencyType = static_cast<EmergencyType>(std::stoi(token)); break;
            case 3: ec.triageLevel = static_cast<TriageLevel>(std::stoi(token)); break;
            case 4: ec.symptoms = token; break;
            case 5: ec.arrivalTime = token; break;
            case 6: ec.arrivalDate = token; break;
            case 7: ec.waitingMinutes = std::stoi(token); break;
        }
        field++;
    }
    
    return ec;
}

// Get triage level name
std::string EmergencyCase::getTriageLevelName() const
{
    switch(triageLevel)
    {
        case RESUSCITATION: return "Level 1 - RESUSCITATION";
        case EMERGENCY:     return "Level 2 - EMERGENCY";
        case URGENT:        return "Level 3 - URGENT";
        case SEMI_URGENT:   return "Level 4 - SEMI-URGENT";
        case NON_URGENT:    return "Level 5 - NON-URGENT";
        default:            return "UNKNOWN";
    }
}

// Get emergency type name
std::string EmergencyCase::getEmergencyTypeName() const
{
    switch(emergencyType)
    {
        case TYPE_A: return "Type A - Cardiac/Respiratory";
        case TYPE_B: return "Type B - Trauma/Accident";
        case TYPE_C: return "Type C - Medical Condition";
        default:     return "Unknown";
    }
}

// Get color for triage level
std::string EmergencyCase::getTriageColor() const
{
    switch(triageLevel)
    {
        case RESUSCITATION: return C_RED;
        case EMERGENCY:     return C_RED;
        case URGENT:        return C_ORANGE;
        case SEMI_URGENT:   return C_YELLOW;
        case NON_URGENT:    return C_GREEN;
        default:            return C_RESET;
    }
}

// Comparison operators for priority queue
bool EmergencyCase::operator<(const EmergencyCase& other) const
{
    // Lower priority score = higher priority
    int thisScore = this->getPriorityScore();
    int otherScore = other.getPriorityScore();
    
    if (thisScore != otherScore)
        return thisScore < otherScore;  // Lower score = higher priority
    
    // If same priority, earlier arrival time = higher priority (FIFO)
    return this->arrivalTime < other.arrivalTime;
}

bool EmergencyCase::operator>(const EmergencyCase& other) const
{
    return other < *this;
}

/* ==================== Helper Functions ==================== */

// Get current time in HH:MM:SS format
std::string getCurrentTimeString()
{
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << localTime->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << localTime->tm_min << ":"
       << std::setfill('0') << std::setw(2) << localTime->tm_sec;
    
    return ss.str();
}

// Get current date in YYYY-MM-DD format
std::string getCurrentDateString()
{
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    
    std::stringstream ss;
    ss << (localTime->tm_year + 1900) << "-"
       << std::setfill('0') << std::setw(2) << (localTime->tm_mon + 1) << "-"
       << std::setfill('0') << std::setw(2) << localTime->tm_mday;
    
    return ss.str();
}

// Calculate waiting time in minutes
int calculateWaitingTime(const std::string& arrivalTime, const std::string& arrivalDate)
{
    // Simple calculation: just return difference in minutes
    // In real implementation, you'd parse the date/time properly
    // For now, return a placeholder
    std::string currentTime = getCurrentTimeString();
    std::string currentDate = getCurrentDateString();
    
    // Simplified: if same date, calculate time difference
    if (arrivalDate == currentDate)
    {
        int arrHour = std::stoi(arrivalTime.substr(0, 2));
        int arrMin = std::stoi(arrivalTime.substr(3, 2));
        
        int currHour = std::stoi(currentTime.substr(0, 2));
        int currMin = std::stoi(currentTime.substr(3, 2));
        
        int totalArrMin = arrHour * 60 + arrMin;
        int totalCurrMin = currHour * 60 + currMin;
        
        return totalCurrMin - totalArrMin;
    }
    
    // Different date: return large number (simplified)
    return 1440; // 24 hours
}