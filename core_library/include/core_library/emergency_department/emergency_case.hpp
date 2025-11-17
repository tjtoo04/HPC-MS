#ifndef EMERGENCY_CASE_HPP
#define EMERGENCY_CASE_HPP

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

// Triage Levels (Standard Emergency Department Classification)
enum TriageLevel
{
    RESUSCITATION = 1,  // Immediate life-threatening
    EMERGENCY = 2,      // Imminently life-threatening  
    URGENT = 3,         // Potentially life-threatening
    SEMI_URGENT = 4,    // Serious but not life-threatening
    NON_URGENT = 5      // Less urgent
};

// Emergency Type Classifications
enum EmergencyType
{
    TYPE_A, // Cardiac/Respiratory emergencies
    TYPE_B, // Trauma/Accidents
    TYPE_C  // Medical conditions
};

struct EmergencyCase
{
    std::string caseID;
    std::string patientName;
    EmergencyType emergencyType;
    TriageLevel triageLevel;
    std::string symptoms;
    std::string arrivalTime;
    std::string arrivalDate;
    int waitingMinutes;  // For priority escalation
    
    // Constructor
    EmergencyCase();
    
    // Parameterized constructor
    EmergencyCase(std::string id, std::string name, EmergencyType type, 
                  TriageLevel triage, std::string symptom);
    
    // Display case information
    void display() const;
    
    // Get priority score (for comparison in heap)
    // Lower score = higher priority
    int getPriorityScore() const;
    
    // Convert to string for file storage
    std::string toString() const;
    
    // Parse from file string
    static EmergencyCase fromString(const std::string& line);
    
    // Get triage level name
    std::string getTriageLevelName() const;
    
    // Get emergency type name
    std::string getEmergencyTypeName() const;
    
    // Get color for triage level (ANSI)
    std::string getTriageColor() const;
    
    // Comparison operator for priority queue
    bool operator<(const EmergencyCase& other) const;
    bool operator>(const EmergencyCase& other) const;
};

// Helper functions
std::string getCurrentTimeString();
std::string getCurrentDateString();
int calculateWaitingTime(const std::string& arrivalTime, const std::string& arrivalDate);

#endif