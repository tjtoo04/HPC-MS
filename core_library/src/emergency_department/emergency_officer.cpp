#include "core_library/emergency_department/emergency_officer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <chrono>
#include <thread>

using namespace std;

// Constructor
EmergencyOfficer::EmergencyOfficer() : nextCaseNumber(1)
{
    loadCasesFromFile("../../data/emergency_cases.txt");
}

// Destructor
EmergencyOfficer::~EmergencyOfficer()
{
    saveCasesToFile("../../data/emergency_cases.txt");
}

// Generate next case ID
string EmergencyOfficer::generateNextCaseID()
{
    stringstream ss;
    ss << "EC" << setfill('0') << setw(4) << nextCaseNumber;
    nextCaseNumber++;
    return ss.str();
}

/* ==================== FILE I/O ==================== */

void EmergencyOfficer::loadCasesFromFile(const string& filename)
{
    ifstream file(filename);
    
    if (!file.is_open())
    {
        cout << C_YELLOW << "\n⚠ INFO: " << C_RESET 
             << "No existing emergency cases file. Starting fresh." << endl;
        return;
    }
    
    string line;
    int count = 0;
    int maxCaseNum = 0;
    
    // Loading bar
    cout << "\n" << C_BOLD << C_CYAN 
         << "🏥 Loading Emergency Cases from Database..." 
         << C_RESET << endl;
    
    const string spinner[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    int spinIndex = 0;
    int total = 0;
    
    // Count total lines first
    {
        string tmp;
        while (getline(file, tmp))
            if (!tmp.empty()) total++;
        file.clear();
        file.seekg(0, ios::beg);
    }
    
    if (total == 0)
    {
        file.close();
        return;
    }
    
    while (getline(file, line))
    {
        if (line.empty()) continue;
        
        EmergencyCase ec = EmergencyCase::fromString(line);
        emergencyQueue.enqueue(ec);
        
        // Extract case number for auto-increment
        if (ec.caseID.length() > 2)
        {
            int caseNum = stoi(ec.caseID.substr(2));
            if (caseNum > maxCaseNum)
                maxCaseNum = caseNum;
        }
        
        count++;
        
        // Progress bar
        float progress = (float)count / total;
        int barWidth = 30;
        int filled = (int)(progress * barWidth);
        
        string bar;
        for (int i = 0; i < barWidth; ++i)
        {
            if (i < filled)
                bar += "█";
            else
                bar += " ";
        }
        
        cout << "\r" << C_BOLD << C_GREEN
             << spinner[spinIndex % 10] << " [" << bar << "] "
             << setw(3) << int(progress * 100) << "% " << C_RESET;
        cout.flush();
        
        spinIndex++;
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
    }
    
    file.close();
    nextCaseNumber = maxCaseNum + 1;
    
    if (count > 0)
    {
        cout << "\r" << C_BOLD << C_GREEN
             << "✅ [██████████████████████████████] 100%" << C_RESET << endl;
        cout << C_GREEN << "✓ SUCCESS: " << C_RESET 
             << "Loaded " << C_BOLD << count << C_RESET 
             << " emergency cases.\n" << endl;
    }
}

void EmergencyOfficer::saveCasesToFile(const string& filename) const
{
    if (emergencyQueue.isEmpty())
        return;
    
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << C_RED << "\n✗ ERROR: " << C_RESET 
             << "Could not save data to file!" << endl;
        return;
    }
    
    const int MAX_CASES = 100;
    EmergencyCase cases[MAX_CASES];
    int count;
    
    emergencyQueue.getAllCases(cases, count);
    
    for (int i = 0; i < count; i++)
    {
        file << cases[i].toString() << endl;
    }
    
    file.close();
}

/* ==================== HELPER FUNCTIONS ==================== */

void EmergencyOfficer::displayTriageInfo() const
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << "                    TRIAGE LEVEL GUIDELINES" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    cout << C_RED << C_BOLD << "\n1. RESUSCITATION (Immediate - Life-threatening)" 
         << C_RESET << endl;
    cout << "   • Cardiac arrest, severe trauma, respiratory arrest\n";
    cout << "   • Target response time: IMMEDIATE\n";
    
    cout << C_RED << "\n2. EMERGENCY (Imminently life-threatening)" 
         << C_RESET << endl;
    cout << "   • Chest pain, severe bleeding, stroke symptoms\n";
    cout << "   • Target response time: 10 minutes\n";
    
    cout << C_YELLOW << "\n3. URGENT (Potentially life-threatening)" 
         << C_RESET << endl;
    cout << "   • Severe pain, high fever, moderate bleeding\n";
    cout << "   • Target response time: 30 minutes\n";
    
    cout << C_YELLOW << "\n4. SEMI-URGENT (Serious but not life-threatening)" 
         << C_RESET << endl;
    cout << "   • Moderate pain, minor fractures, minor burns\n";
    cout << "   • Target response time: 60 minutes\n";
    
    cout << C_GREEN << "\n5. NON-URGENT (Less urgent)" 
         << C_RESET << endl;
    cout << "   • Minor injuries, cold/flu symptoms, minor cuts\n";
    cout << "   • Target response time: 120 minutes\n";
    
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
}

void EmergencyOfficer::displayEmergencyTypeInfo() const
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << "                  EMERGENCY TYPE CLASSIFICATIONS" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    cout << C_RED << "\nType A - Cardiac/Respiratory Emergencies:" 
         << C_RESET << endl;
    cout << "• Heart attack, cardiac arrest, arrhythmia\n";
    cout << "• Severe asthma, respiratory failure, choking\n";
    cout << "• Anaphylaxis, severe allergic reactions\n";
    
    cout << C_YELLOW << "\nType B - Trauma/Accidents:" 
         << C_RESET << endl;
    cout << "• Motor vehicle accidents, falls from height\n";
    cout << "• Gunshot wounds, stab wounds, severe burns\n";
    cout << "• Head trauma, spinal injuries, fractures\n";
    
    cout << C_GREEN << "\nType C - Medical Conditions:" 
         << C_RESET << endl;
    cout << "• Stroke, seizures, loss of consciousness\n";
    cout << "• Severe infections, diabetic emergencies\n";
    cout << "• Poisoning, overdose, severe pain\n";
    
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
}

TriageLevel EmergencyOfficer::selectTriageLevel() const
{
    cout << "\n" << C_BOLD << "Select Triage Level:" << C_RESET << endl;
    cout << "  " << C_RED << C_BOLD << "1." << C_RESET 
         << " RESUSCITATION (Immediate)\n";
    cout << "  " << C_RED << "2." << C_RESET 
         << " EMERGENCY (Imminently life-threatening)\n";
    cout << "  " << C_YELLOW << "3." << C_RESET 
         << " URGENT (Potentially life-threatening)\n";
    cout << "  " << C_YELLOW << "4." << C_RESET 
         << " SEMI-URGENT (Serious)\n";
    cout << "  " << C_GREEN << "5." << C_RESET 
         << " NON-URGENT (Less urgent)\n";
    
    int choice;
    while (true)
    {
        cout << "\nEnter triage level (1-5): ";
        if (cin >> choice && choice >= 1 && choice <= 5)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return static_cast<TriageLevel>(choice);
        }
        
        cout << C_RED << "Invalid input! Please enter 1-5." 
             << C_RESET << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

EmergencyType EmergencyOfficer::selectEmergencyType() const
{
    cout << "\n" << C_BOLD << "Select Emergency Type:" << C_RESET << endl;
    cout << "  " << C_RED << "A." << C_RESET 
         << " Cardiac/Respiratory\n";
    cout << "  " << C_YELLOW << "B." << C_RESET 
         << " Trauma/Accident\n";
    cout << "  " << C_GREEN << "C." << C_RESET 
         << " Medical Condition\n";
    
    char choice;
    while (true)
    {
        cout << "\nEnter type (A/B/C): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        choice = toupper(choice);
        if (choice == 'A') return TYPE_A;
        if (choice == 'B') return TYPE_B;
        if (choice == 'C') return TYPE_C;
        
        cout << C_RED << "Invalid input! Please enter A, B, or C." 
             << C_RESET << endl;
    }
}

/* ==================== FUNCTIONALITY 1: LOG EMERGENCY CASE ==================== */

void EmergencyOfficer::logEmergencyCase()
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           LOG NEW EMERGENCY CASE" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    // Check if queue is full
    if (emergencyQueue.isFull())
    {
        cout << C_RED << "\n✗ ERROR: " << C_RESET 
             << "Emergency queue is full! Cannot log new cases." << endl;
        return;
    }
    
    // Auto-generate case ID
    string caseID = generateNextCaseID();
    cout << C_GREEN << "\n✓ Case ID auto-generated: " << C_BOLD 
         << caseID << C_RESET << endl;
    
    // Get patient name
    string patientName;
    do
    {
        cout << "\n" << C_YELLOW << "Enter Patient Name: " << C_RESET;
        getline(cin, patientName);
        
        // Trim whitespace
        patientName.erase(0, patientName.find_first_not_of(" \t"));
        patientName.erase(patientName.find_last_not_of(" \t") + 1);
        
        if (patientName.empty())
        {
            cout << C_RED << "⚠ ERROR: Name cannot be empty!" 
                 << C_RESET << endl;
        }
    } while (patientName.empty());
    
    // Display triage information
    displayTriageInfo();
    TriageLevel triage = selectTriageLevel();
    
    // Display emergency type information
    displayEmergencyTypeInfo();
    EmergencyType type = selectEmergencyType();
    
    // Get symptoms
    string symptoms;
    cout << "\n" << C_YELLOW << "Enter Symptoms/Complaint: " << C_RESET;
    getline(cin, symptoms);
    
    // Create emergency case
    EmergencyCase newCase(caseID, patientName, type, triage, symptoms);
    
    // Add to priority queue
    if (emergencyQueue.enqueue(newCase))
    {
        cout << "\n" << C_GREEN << string(70, '=') << C_RESET << endl;
        cout << C_GREEN << C_BOLD << "  ✓ EMERGENCY CASE LOGGED SUCCESSFULLY!" 
             << C_RESET << endl;
        cout << C_GREEN << string(70, '=') << C_RESET << endl;
        
        cout << "\n" << C_BOLD << "Case Details:" << C_RESET << endl;
        newCase.display();
        
        // Show queue position
        cout << "\n" << C_CYAN << "📊 Current Queue Status:" << C_RESET << endl;
        cout << "   Total pending cases: " << C_BOLD 
             << emergencyQueue.getSize() << C_RESET << endl;
        
        // Check if this is highest priority
        EmergencyCase highestPriority;
        emergencyQueue.peek(highestPriority);
        if (highestPriority.caseID == caseID)
        {
            cout << C_RED << C_BOLD 
                 << "   ⚠ ALERT: This is now the HIGHEST PRIORITY case!" 
                 << C_RESET << endl;
        }
    }
}

/* ==================== FUNCTIONALITY 2: PROCESS MOST CRITICAL CASE ==================== */

void EmergencyOfficer::processMostCriticalCase()
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           PROCESS MOST CRITICAL CASE" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    if (emergencyQueue.isEmpty())
    {
        cout << C_YELLOW << "\n⚠ INFO: " << C_RESET 
             << "No pending emergency cases in queue." << endl;
        return;
    }
    
    // Update waiting times first
    emergencyQueue.updateWaitingTimes();
    
    // Peek at highest priority case
    EmergencyCase criticalCase;
    emergencyQueue.peek(criticalCase);
    
    cout << "\n" << C_BOLD << C_RED 
         << "🚨 HIGHEST PRIORITY CASE:" << C_RESET << endl;
    criticalCase.display();
    
    // Confirmation
    cout << "\n" << C_YELLOW 
         << "Proceed with treating this patient? (Y/N): " 
         << C_RESET;
    char confirm;
    cin >> confirm;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (toupper(confirm) != 'Y')
    {
        cout << C_YELLOW << "\n✓ Treatment cancelled." << C_RESET << endl;
        return;
    }
    
    // Process (remove from queue)
    EmergencyCase processedCase;
    if (emergencyQueue.dequeue(processedCase))
    {
        cout << "\n" << C_GREEN << string(70, '=') << C_RESET << endl;
        cout << C_GREEN << C_BOLD 
             << "  ✓ PATIENT SENT TO TREATMENT!" 
             << C_RESET << endl;
        cout << C_GREEN << string(70, '=') << C_RESET << endl;
        
        cout << "\n" << C_BOLD << "Processed Case:" << C_RESET << endl;
        cout << "  Case ID: " << processedCase.caseID << endl;
        cout << "  Patient: " << processedCase.patientName << endl;
        cout << "  Triage: " << processedCase.getTriageLevelName() << endl;
        cout << "  Total Waiting Time: " << processedCase.waitingMinutes 
             << " minutes" << endl;
        
        cout << "\n" << C_CYAN << "📊 Remaining Cases: " 
             << emergencyQueue.getSize() << C_RESET << endl;
        
        // Show next highest priority if queue not empty
        if (!emergencyQueue.isEmpty())
        {
            EmergencyCase nextCase;
            emergencyQueue.peek(nextCase);
            cout << "\n" << C_YELLOW << "Next Priority Case:" 
                 << C_RESET << endl;
            cout << "  Case ID: " << nextCase.caseID << endl;
            cout << "  Patient: " << nextCase.patientName << endl;
            cout << "  Triage: " << nextCase.getTriageLevelName() << endl;
        }
    }
}

/* ==================== FUNCTIONALITY 3: VIEW PENDING CASES ==================== */

void EmergencyOfficer::viewPendingCases()
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           PENDING EMERGENCY CASES" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    // Update waiting times before displaying
    emergencyQueue.updateWaitingTimes();
    
    if (emergencyQueue.isEmpty())
    {
        cout << C_YELLOW << "\n⚠ INFO: " << C_RESET 
             << "No pending emergency cases." << endl;
        return;
    }
    
    // Display all cases in priority order
    emergencyQueue.display();
}

/* ==================== ADDITIONAL FEATURE 1: PRIORITY ESCALATION ==================== */

void EmergencyOfficer::checkPriorityEscalation()
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           PRIORITY ESCALATION CHECK" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    if (emergencyQueue.isEmpty())
    {
        cout << C_GREEN << "\n✓ No cases waiting." << C_RESET << endl;
        return;
    }
    
    // Update waiting times
    emergencyQueue.updateWaitingTimes();
    
    const int MAX_CASES = 100;
    EmergencyCase cases[MAX_CASES];
    int count;
    emergencyQueue.getAllCases(cases, count);
    
    // Check for cases needing escalation
    int escalatedCount = 0;
    
    cout << "\n" << C_BOLD << "Checking waiting times..." << C_RESET << endl;
    
    for (int i = 0; i < count; i++)
    {
        // Escalation thresholds (in minutes)
        int threshold = 0;
        switch(cases[i].triageLevel)
        {
            case RESUSCITATION: threshold = 0; break;   // Immediate
            case EMERGENCY:     threshold = 10; break;  // 10 min
            case URGENT:        threshold = 30; break;  // 30 min
            case SEMI_URGENT:   threshold = 60; break;  // 60 min
            case NON_URGENT:    threshold = 120; break; // 2 hours
        }
        
        if (cases[i].waitingMinutes > threshold && 
            cases[i].triageLevel != RESUSCITATION)
        {
            cout << C_RED << "\n⚠ ESCALATION ALERT:" << C_RESET << endl;
            cout << "  Case: " << cases[i].caseID 
                 << " (" << cases[i].patientName << ")" << endl;
            cout << "  Current Triage: " << cases[i].getTriageLevelName() 
                 << endl;
            cout << "  Waiting: " << cases[i].waitingMinutes 
                 << " minutes (Threshold: " << threshold << " min)" << endl;
            escalatedCount++;
        }
    }
    
    if (escalatedCount == 0)
    {
        cout << C_GREEN << "\n✓ All cases within acceptable waiting times." 
             << C_RESET << endl;
    }
    else
    {
        cout << "\n" << C_RED << C_BOLD << "Total cases requiring attention: " 
             << escalatedCount << C_RESET << endl;
    }
}

/* ==================== ADDITIONAL FEATURE 2: SEARCH CASE ==================== */

void EmergencyOfficer::searchCase()
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           SEARCH EMERGENCY CASE" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    if (emergencyQueue.isEmpty())
    {
        cout << C_YELLOW << "\n⚠ No cases in queue." << C_RESET << endl;
        return;
    }
    
    cout << "\n" << C_YELLOW << "Enter Case ID to search: " << C_RESET;
    string searchID;
    getline(cin, searchID);
    
    EmergencyCase foundCase;
    if (emergencyQueue.findCase(searchID, foundCase))
    {
        cout << "\n" << C_GREEN << "✓ Case found!" << C_RESET << endl;
        foundCase.display();
    }
    else
    {
        cout << C_RED << "\n✗ Case ID not found: " << searchID 
             << C_RESET << endl;
    }
}

/* ==================== ADDITIONAL FEATURE 3: STATISTICS DASHBOARD ==================== */

void EmergencyOfficer::displayDashboard()
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           EMERGENCY DEPARTMENT DASHBOARD" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    if (emergencyQueue.isEmpty())
    {
        cout << C_GREEN << "\n✓ No pending cases. Department clear!" 
             << C_RESET << endl;
        return;
    }
    
    const int MAX_CASES = 100;
    EmergencyCase cases[MAX_CASES];
    int count;
    emergencyQueue.getAllCases(cases, count);
    
    // Count by triage level
    int countByTriage[6] = {0}; // Index 0 unused, 1-5 for triage levels
    int countByType[3] = {0};   // TYPE_A, TYPE_B, TYPE_C
    int totalWaitTime = 0;
    int maxWaitTime = 0;
    
    for (int i = 0; i < count; i++)
    {
        countByTriage[cases[i].triageLevel]++;
        countByType[cases[i].emergencyType]++;
        totalWaitTime += cases[i].waitingMinutes;
        if (cases[i].waitingMinutes > maxWaitTime)
            maxWaitTime = cases[i].waitingMinutes;
    }
    
    cout << "\n" << C_BOLD << "📊 QUEUE STATISTICS:" << C_RESET << endl;
    cout << "  Total Pending Cases: " << C_BOLD << count << C_RESET << endl;
    cout << "  Average Wait Time: " << (count > 0 ? totalWaitTime/count : 0) 
         << " minutes" << endl;
    cout << "  Longest Wait: " << maxWaitTime << " minutes" << endl;
    
    cout << "\n" << C_BOLD << "📋 BY TRIAGE LEVEL:" << C_RESET << endl;
    cout << "  " << C_RED << "Level 1 (Resuscitation): " << C_RESET 
         << countByTriage[1] << endl;
    cout << "  " << C_RED << "Level 2 (Emergency):     " << C_RESET 
         << countByTriage[2] << endl;
    cout << "  " << C_YELLOW << "Level 3 (Urgent):        " << C_RESET 
         << countByTriage[3] << endl;
    cout << "  " << C_YELLOW << "Level 4 (Semi-Urgent):   " << C_RESET 
         << countByTriage[4] << endl;
    cout << "  " << C_GREEN << "Level 5 (Non-Urgent):    " << C_RESET 
         << countByTriage[5] << endl;
    
    cout << "\n" << C_BOLD << "🏥 BY EMERGENCY TYPE:" << C_RESET << endl;
    cout << "  Type A (Cardiac/Respiratory): " << countByType[0] << endl;
    cout << "  Type B (Trauma/Accident):     " << countByType[1] << endl;
    cout << "  Type C (Medical Condition):   " << countByType[2] << endl;
    
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
}

/* ==================== ADDITIONAL FEATURE 4: TRIAGE GUIDELINES ==================== */

void EmergencyOfficer::showTriageGuidelines()
{
    displayTriageInfo();
    
    cout << "\n" << C_BOLD << "Press Enter to continue..." << C_RESET;
    cin.get();
}

/* ==================== ADDITIONAL FEATURE 5: BATCH PROCESS ==================== */

void EmergencyOfficer::batchProcessCases()
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           BATCH PROCESS CASES" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    if (emergencyQueue.isEmpty())
    {
        cout << C_YELLOW << "\n⚠ No cases to process." << C_RESET << endl;
        return;
    }
    
    int numberToProcess;
    cout << "\nTotal cases in queue: " << emergencyQueue.getSize() << endl;
    cout << C_YELLOW << "How many cases to process? " << C_RESET;
    cin >> numberToProcess;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    if (numberToProcess > emergencyQueue.getSize())
        numberToProcess = emergencyQueue.getSize();
    
    if (numberToProcess <= 0)
    {
        cout << C_RED << "Invalid number." << C_RESET << endl;
        return;
    }
    
    cout << "\n" << C_BOLD << "Processing " << numberToProcess 
         << " cases..." << C_RESET << endl;
    
    for (int i = 0; i < numberToProcess; i++)
    {
        EmergencyCase processedCase;
        if (emergencyQueue.dequeue(processedCase))
        {
            cout << "\n" << C_GREEN << "✓ Case " << (i+1) << "/" 
                 << numberToProcess << " - " << processedCase.caseID 
                 << " (" << processedCase.patientName << ") processed." 
                 << C_RESET << endl;
        }
    }
    
    cout << "\n" << C_GREEN << C_BOLD << "✓ Batch processing complete!" 
         << C_RESET << endl;
    cout << "Remaining cases: " << emergencyQueue.getSize() << endl;
}

/* ==================== ADDITIONAL FEATURE 6: EXPORT TO FILE ==================== */

void EmergencyOfficer::exportCasesToFile()
{
    cout << "\n" << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           EXPORT CASES TO REPORT" 
         << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;
    
    if (emergencyQueue.isEmpty())
    {
        cout << C_YELLOW << "\n⚠ No cases to export." << C_RESET << endl;
        return;
    }
    
    string filename = "../../data/emergency_report_" + getCurrentDateString() + ".txt";
    
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << C_RED << "\n✗ ERROR: Could not create report file." 
             << C_RESET << endl;
        return;
    }
    
    file << "===================================\n";
    file << "  EMERGENCY DEPARTMENT REPORT\n";
    file << "  Date: " << getCurrentDateString() << "\n";
    file << "  Time: " << getCurrentTimeString() << "\n";
    file << "===================================\n\n";
    
    const int MAX_CASES = 100;
    EmergencyCase cases[MAX_CASES];
    int count;
    emergencyQueue.getAllCases(cases, count);
    
    file << "Total Pending Cases: " << count << "\n\n";
    
    for (int i = 0; i < count; i++)
    {
        file << "Case " << (i+1) << ":\n";
        file << "  ID: " << cases[i].caseID << "\n";
        file << "  Patient: " << cases[i].patientName << "\n";
        file << "  Triage: " << cases[i].getTriageLevelName() << "\n";
        file << "  Type: " << cases[i].getEmergencyTypeName() << "\n";
        file << "  Symptoms: " << cases[i].symptoms << "\n";
        file << "  Arrival: " << cases[i].arrivalDate << " " 
             << cases[i].arrivalTime << "\n";
        file << "  Waiting: " << cases[i].waitingMinutes << " minutes\n\n";
    }
    
    file.close();
    
    cout << C_GREEN << "\n✓ Report exported successfully!" << C_RESET << endl;
    cout << "Location: " << C_BOLD << filename << C_RESET << endl;
}

/* ==================== MENU AND MAIN LOOP ==================== */

void EmergencyOfficer::displayMenu()
{
    cout << "\n" << C_BOLD << C_BLUE << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_BLUE 
         << "     🏥 EMERGENCY DEPARTMENT OFFICER MANAGEMENT 🏥" 
         << C_RESET << endl;
    cout << C_BOLD << C_BLUE << string(70, '=') << C_RESET << endl;
    
    cout << "\n" << C_BOLD << "REQUIRED FUNCTIONALITIES:" << C_RESET << endl;
    cout << "  " << C_CYAN << "1." << C_RESET << " Log Emergency Case\n";
    cout << "  " << C_CYAN << "2." << C_RESET 
         << " Process Most Critical Case\n";
    cout << "  " << C_CYAN << "3." << C_RESET << " View Pending Cases\n";
    
    cout << "\n" << C_BOLD << "ADDITIONAL FEATURES:" << C_RESET << endl;
    cout << "  " << C_CYAN << "4." << C_RESET 
         << " Check Priority Escalation\n";
    cout << "  " << C_CYAN << "5." << C_RESET << " Search Case\n";
    cout << "  " << C_CYAN << "6." << C_RESET << " Statistics Dashboard\n";
    cout << "  " << C_CYAN << "7." << C_RESET << " Triage Guidelines\n";
    cout << "  " << C_CYAN << "8." << C_RESET << " Batch Process Cases\n";
    cout << "  " << C_CYAN << "9." << C_RESET << " Export Report\n";
    cout << "  " << C_RED << "0." << C_RESET << " Exit & Save\n";
    
    cout << C_BOLD << C_BLUE << string(70, '=') << C_RESET << endl;
    cout << "Enter your choice: ";
}

void EmergencyOfficer::run()
{
    int choice;
    bool running = true;
    
    while (running)
    {
        displayMenu();
        
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << C_RED << "\n✗ Invalid input! Please enter 0-9." 
                 << C_RESET << endl;
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice)
        {
            case 1:
                logEmergencyCase();
                break;
            case 2:
                processMostCriticalCase();
                break;
            case 3:
                viewPendingCases();
                break;
            case 4:
                checkPriorityEscalation();
                break;
            case 5:
                searchCase();
                break;
            case 6:
                displayDashboard();
                break;
            case 7:
                showTriageGuidelines();
                break;
            case 8:
                batchProcessCases();
                break;
            case 9:
                exportCasesToFile();
                break;
            case 0:
                cout << "\n" << C_YELLOW << "Saving emergency cases..." 
                     << C_RESET << endl;
                saveCasesToFile("../../data/emergency_cases.txt");
                cout << C_GREEN 
                     << "\n✓ Data saved successfully to emergency_cases.txt" 
                     << C_RESET << endl;
                cout << C_GREEN 
                     << "\n✓ Thank you for using Emergency Department System!" 
                     << C_RESET << endl;
                running = false;
                break;
            default:
                cout << C_RED << "\n✗ Invalid choice! Please select 0-9." 
                     << C_RESET << endl;
        }
        
        if (running)
        {
            cout << "\n" << C_YELLOW << "Press Enter to continue..." 
                 << C_RESET;
            cin.get();
        }
    }
}