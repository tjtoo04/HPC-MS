#define NOMINMAX
#define NOBYTE
#ifdef _WIN32
#include <windows.h>
#endif
#include "core_library/ambulance_dispatcher.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <thread>
using namespace std;

// Constructor
AmbulanceDispatcher::AmbulanceDispatcher() : nextAmbulanceNumber(1), shiftDurationHours(8)
{
    loadAmbulancesFromFile("ambulances.txt");
}

// Destructor
AmbulanceDispatcher::~AmbulanceDispatcher()
{
    saveAmbulancesToFile("ambulances.txt");
}

// Load ambulances from file
void AmbulanceDispatcher::loadAmbulancesFromFile(const string &filename)
{

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << C_YELLOW << "\n⚠ INFO: " << C_RESET << "No existing data file. Starting fresh system." << endl;
        return;
    }

    string line;
    int count = 0;
    int maxAmbNum = 0;

    // Loading bar
    cout << "\n"
         << C_BOLD << C_CYAN << "🚑 Loading Ambulances from Database..." << C_RESET << endl;

    const string blocks = "▏▎▍▌▋▊▉█";
    const string spinner[] = {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"};
    int spinIndex = 0;
    int total = 0;
    {
        string tmp;
        while (getline(file, tmp))
            total++;
        file.clear();
        file.seekg(0, ios::beg);
    }

    while (getline(file, line))
    {
        stringstream ss(line);
        string vehicleID, ambulanceID, driver, status, scheduleDate, startTime, endTime;

        if (getline(ss, vehicleID, ',') &&
            getline(ss, ambulanceID, ',') &&
            getline(ss, driver, ',') &&
            getline(ss, status, ',') &&
            getline(ss, scheduleDate, ',') &&
            getline(ss, startTime, ',') &&
            getline(ss, endTime, ','))
        {

            // Extract vehicle ID for auto-increment
            if (vehicleID.length() > 3)
            {
                int ambNum = stoi(vehicleID.substr(3));
                if (ambNum > maxAmbNum)
                    maxAmbNum = ambNum;
            }

            Ambulance ambulance(vehicleID, ambulanceID, driver, status, scheduleDate, startTime, endTime);
            ambulanceQueue.enqueue(ambulance);
            count++;
        }

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
    nextAmbulanceNumber = maxAmbNum + 1;

    if (count > 0)
    {
        cout << "\r" << C_BOLD << C_GREEN
             << "✅ [██████████████████████████████] 100%" << C_RESET << endl;
        cout << C_GREEN << "\n✓ SUCCESS: " << C_RESET
             << "Loaded " << C_BOLD << count << C_RESET
             << " ambulances from database.\n"
             << endl;
    }
}

// Save ambulances to file
void AmbulanceDispatcher::saveAmbulancesToFile(const string &filename) const
{
    if (ambulanceQueue.isEmpty())
    {
        return;
    }

    ofstream file(filename);
    if (!file.is_open())
    {
        cout << C_RED << "\n✗ ERROR: " << C_RESET << "Could not save data to file!" << endl;
        return;
    }

    const int MAX_AMB = 50;
    Ambulance ambulances[MAX_AMB];
    int count;

    ambulanceQueue.getAllAmbulances(ambulances, count);

    for (int i = 0; i < count; i++)
    {
        file << ambulances[i].vehicleID << ","
             << ambulances[i].ambulanceID << ","
             << ambulances[i].driverName << ","
             << ambulances[i].status << ","
             << ambulances[i].scheduleDate << ","
             << ambulances[i].shiftStartTime << ","
             << ambulances[i].shiftEndTime << endl;
    }

    file.close();
}

// Generate ambulance ID
string AmbulanceDispatcher::generateNextAmbulanceID()
{
    stringstream ss;
    ss << "AMB" << setfill('0') << setw(3) << nextAmbulanceNumber;
    nextAmbulanceNumber++;
    return ss.str();
}

// Check if rotation is needed based on duty hours
bool AmbulanceDispatcher::isRotationNeeded() const
{
    Ambulance currentDuty;
    if (!ambulanceQueue.getFront(currentDuty))
    {
        return false;
    }

    // Check if current shift has ended
    string currentDate = getCurrentDateString();
    string currentTime = getCurrentTimeString();
    if (currentDate > currentDuty.scheduleDate)
    {
        return true;
    }

    // Same date, check time
    if (currentDate == currentDuty.scheduleDate)
    {
        return (currentTime >= currentDuty.shiftEndTime && currentDuty.shiftEndTime != "--:--" && currentDuty.shiftEndTime > currentDuty.shiftStartTime);
    }

    return false;
}

// Format duration display
string AmbulanceDispatcher::formatDuration(int hours) const
{
    if (hours == 0)
        return "0 hrs";
    return to_string(hours) + " hrs";
}

/* ========================== Functionality 1: Register Ambulance ================================= */
void AmbulanceDispatcher::registerAmbulance()
{
    cout << "\n"
         << C_CYAN << string(60, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           REGISTER NEW AMBULANCE" << C_RESET << endl;
    cout << C_CYAN << string(60, '=') << C_RESET << endl;

    string ambulanceID, driver, startTime;

    // Auto-generate ambulance ID
    string vehicleID = generateNextAmbulanceID();
    cout << C_GREEN << "\n✓ Ambulance ID auto-generated: " << C_BOLD << vehicleID << C_RESET << endl;

    do
    {
        cout << "\n"
             << C_YELLOW << "Enter Ambulance Registration Number" << C_RESET
             << " (e.g., KL-1234-A): ";
        getline(cin, ambulanceID);

        ambulanceID.erase(0, ambulanceID.find_first_not_of(" \t"));
        ambulanceID.erase(ambulanceID.find_last_not_of(" \t") + 1);

        if (ambulanceID.empty())
        {
            cout << C_RED << "⚠ ERROR: Registration number cannot be empty. Please enter again." << C_RESET << endl;
        }
    } while (ambulanceID.empty());

    do
    {
        cout << C_YELLOW << "Enter Driver Full Name: " << C_RESET;
        getline(cin, driver);

        driver.erase(0, driver.find_first_not_of(" \t"));
        driver.erase(driver.find_last_not_of(" \t") + 1);

        if (driver.empty())
        {
            cout << C_RED << "⚠ ERROR: Driver name cannot be empty. Please enter again." << C_RESET << endl;
        }
    } while (driver.empty());

    // Determine initial status
    string status = "Standby";
    string shiftStart = "--:--";
    string shiftEnd = "--:--";
    string scheduleDate = getCurrentDateString();
    if (ambulanceQueue.isEmpty())
    {
        // First ambulance - assign current time as start
        status = "On Duty";
        shiftStart = getCurrentTimeString();
        shiftEnd = addHoursToTime(shiftStart, shiftDurationHours);

        if (shiftEnd < shiftStart)
        {
            addHoursToDateTime(scheduleDate, shiftStart, 0);
        }

        cout << C_GREEN << "\n✓ First ambulance registered!" << C_RESET << endl;
        cout << C_GREEN << "  Status: " << C_BOLD << "On Duty" << C_RESET << endl;
        cout << C_GREEN << "  Date: " << C_BOLD << scheduleDate << " (" << getDayOfWeek(scheduleDate) << ")" << C_RESET << endl;
        cout << C_GREEN << "  Shift: " << C_BOLD << shiftStart << " - " << shiftEnd
             << C_RESET << " (" << shiftDurationHours << " hours)" << endl;
    }
    else
    {
        // Calculate next available shift time based on last ambulance
        const int MAX_AMB = 50;
        Ambulance ambulances[MAX_AMB];
        int count;
        ambulanceQueue.getAllAmbulances(ambulances, count);

        // Find the latest shift end time
        if (count > 0)
        {
            Ambulance lastAmbulance = ambulances[count - 1];
            if (lastAmbulance.shiftEndTime != "--:--")
            {
                // Base date from previous ambulance
                scheduleDate = lastAmbulance.scheduleDate;
                shiftStart = lastAmbulance.shiftEndTime;
                shiftEnd = addHoursToTime(shiftStart, shiftDurationHours);

                if (shiftStart == "00:00" || (shiftEnd < shiftStart && shiftEnd != "00:00"))
                {
                    addHoursToDateTime(scheduleDate, shiftStart, 24); // Move to next day
                }
            }
        }

        cout << C_YELLOW << "\n✓ Status: " << C_BOLD << "Standby"
             << C_RESET << " (Scheduled for rotation)" << endl;
        if (shiftStart != "--:--")
        {
            cout << C_YELLOW << "  Scheduled Shift: " << C_BOLD << shiftStart << " - " << shiftEnd
                 << C_RESET << " (" << shiftDurationHours << " hours)" << endl;
        }
    }

    // Create ambulance object
    Ambulance newAmbulance(vehicleID, ambulanceID, driver, status, scheduleDate, shiftStart, shiftEnd);

    // Add to queue
    ambulanceQueue.enqueue(newAmbulance);

    cout << C_GREEN << "\n✓ SUCCESS: " << C_RESET << "Ambulance "
         << C_BOLD << newAmbulance.ambulanceID << C_RESET
         << " registered and added to rotation!" << endl;

    cout << "\n"
         << C_GREEN << "═══════════════════════════════════════════════" << C_RESET << endl;
    cout << C_GREEN << "  Ambulance successfully added to the schedule!" << C_RESET << endl;
    cout << C_GREEN << "═══════════════════════════════════════════════" << C_RESET << endl;

    // Auto-check if system needs balancing
    autoScheduleCheck();
}

// Auto-schedule check
void AmbulanceDispatcher::autoScheduleCheck()
{
    if (isRotationNeeded())
    {
        Ambulance current;
        ambulanceQueue.getFront(current);
        cout << "\n"
             << C_RED << C_BOLD << "⚠ AUTO-SCHEDULE ALERT!" << C_RESET << endl;
        cout << C_RED << "  Current ambulance (" << current.vehicleID
             << ") shift ended at " << current.shiftEndTime << C_RESET << endl;
        cout << C_YELLOW << "  Please rotate shift to maintain continuous coverage!" << C_RESET << endl;
    }
}

/* ========================== Functionality 2: Rotate Ambulance Shift ========================= */
void AmbulanceDispatcher::rotateAmbulanceShift()
{
    cout << "\n"
         << C_CYAN << string(60, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           AMBULANCE SHIFT ROTATION" << C_RESET << endl;
    cout << C_CYAN << string(60, '=') << C_RESET << endl;

    if (ambulanceQueue.isEmpty())
    {
        cout << C_RED << "\n✗ ERROR: " << C_RESET << "No ambulances available for rotation!" << endl;
        return;
    }

    if (ambulanceQueue.getSize() == 1)
    {
        cout << C_YELLOW << "\n⚠ INFO: " << C_RESET << "Only one ambulance in system." << endl;
        cout << "Extending current shift by " << shiftDurationHours << " hours..." << endl;

        Ambulance current;
        ambulanceQueue.getFront(current);
        current.shiftStartTime = current.shiftEndTime;
        current.shiftEndTime = addHoursToTime(current.shiftStartTime, shiftDurationHours);
        ambulanceQueue.updateFront(current);

        cout << C_GREEN << "\n✓ Shift extended: " << current.shiftStartTime
             << " - " << current.shiftEndTime << C_RESET << endl;
        return;
    }

    // Display current ambulance info
    Ambulance currentDuty;
    ambulanceQueue.getFront(currentDuty);
    string currentTime = getCurrentTimeString();

    cout << "\n"
         << C_BOLD << "Current Schedule:" << C_RESET << endl;
    cout << "  Current Time: " << C_CYAN << currentTime << C_RESET << endl;
    cout << "\n"
         << C_BOLD << "Ambulance on Duty:" << C_RESET << endl;
    cout << "  Vehicle ID: " << C_GREEN << currentDuty.vehicleID << C_RESET << endl;
    cout << "  Registration: " << currentDuty.ambulanceID << endl;
    cout << "  Driver: " << currentDuty.driverName << endl;
    cout << "  Shift: " << C_YELLOW << currentDuty.shiftStartTime << " - "
         << currentDuty.shiftEndTime << C_RESET << " (" << currentDuty.getShiftDurationHours() << " hrs)" << endl;

    // Check if shift has ended
    bool shiftEnded = (currentTime >= currentDuty.shiftEndTime && currentDuty.shiftEndTime != "--:--");

    if (shiftEnded)
    {
        cout << C_GREEN << "\n✓ Current shift has ended. Rotation recommended." << C_RESET << endl;
    }
    else
    {
        cout << "\n"
             << C_YELLOW << "⚠ NOTICE: " << C_RESET
             << "Current shift ends at " << C_YELLOW << currentDuty.shiftEndTime << C_RESET << endl;
        cout << "Current time is " << C_CYAN << currentTime << C_RESET << ". Shift is still active." << endl;
    }

    // Ask for rotation type
    cout << "\n"
         << C_BOLD << "Select Rotation Type:" << C_RESET << endl;
    cout << "  " << C_CYAN << "1." << C_RESET << " Normal Rotation (Follow original schedule)" << endl;
    cout << "  " << C_CYAN << "2." << C_RESET << " Dynamic Rotation (Adjust to current time)" << endl;
    cout << "  " << C_CYAN << "3." << C_RESET << " Cancel" << endl;
    cout << "\nEnter choice (1-3): ";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        normalRotate();
        break;
    case 2:
        dynamicRotate();
        break;
    case 3:
        cout << C_YELLOW << "\n✓ Rotation cancelled." << C_RESET << endl;
        return;
    default:
        cout << C_RED << "\n✗ Invalid choice. Rotation cancelled." << C_RESET << endl;
        return;
    }

    displayUpcomingRotation();
}

// Normal rotation
void AmbulanceDispatcher::normalRotate()
{
    Ambulance currentDuty;
    ambulanceQueue.getFront(currentDuty);

    // Get all ambulances to properly update the rotated one
    const int MAX_AMB = 50;
    Ambulance ambulances[MAX_AMB];
    int count;
    ambulanceQueue.getAllAmbulances(ambulances, count);

    // The second ambulance = 1
    string nextShiftStart = (count > 1) ? ambulances[1].shiftStartTime : addHoursToTime(currentDuty.shiftEndTime, 0);
    string nextShiftEnd = (count > 1) ? ambulances[1].shiftEndTime : addHoursToTime(nextShiftStart, shiftDurationHours);

    // Calculate new schedule for the ambulance moving to rear
    string rotatedDate = ambulances[count - 1].scheduleDate;
    string rotatedStartTime = ambulances[count - 1].shiftEndTime;
    string rotatedEndTime = addHoursToTime(rotatedStartTime, shiftDurationHours);

    if (rotatedStartTime == "00:00" || (rotatedEndTime < rotatedStartTime && rotatedEndTime != "00:00"))
    {
        // This shift (midnight or more) = next day
        string tempDate = rotatedDate;
        addHoursToDateTime(tempDate, rotatedStartTime, 0);
        addHoursToDateTime(rotatedDate, rotatedStartTime, 24);
    }

    ambulanceQueue.rotate();

    // Update new on-duty ambulance with its original schedule
    Ambulance newDuty;
    ambulanceQueue.getFront(newDuty);
    newDuty.status = "On Duty";
    newDuty.shiftStartTime = nextShiftStart;
    newDuty.shiftEndTime = nextShiftEnd;
    ambulanceQueue.updateFront(newDuty);

    Ambulance updatedAmbulances[MAX_AMB];
    ambulanceQueue.getAllAmbulances(updatedAmbulances, count);

    updatedAmbulances[count - 1].scheduleDate = rotatedDate;
    updatedAmbulances[count - 1].shiftStartTime = rotatedStartTime;
    updatedAmbulances[count - 1].shiftEndTime = rotatedEndTime;

    // Rebuild the queue with updated schedules
    while (!ambulanceQueue.isEmpty())
    {
        Ambulance temp;
        ambulanceQueue.dequeue(temp);
    }

    for (int i = 0; i < count; i++)
    {
        ambulanceQueue.enqueue(updatedAmbulances[i]);
    }

    cout << "\n"
         << C_GREEN << "═══════════════════════════════════════════════" << C_RESET << endl;
    cout << C_GREEN << C_BOLD << "  ✓ NORMAL ROTATION COMPLETED!" << C_RESET << endl;
    cout << C_GREEN << "═══════════════════════════════════════════════" << C_RESET << endl;

    cout << "\n"
         << C_BOLD << "Shift Changes:" << C_RESET << endl;
    cout << "  " << currentDuty.vehicleID << " (" << currentDuty.driverName << ")" << endl;
    cout << "    Status: " << C_YELLOW << "On Duty → Standby" << C_RESET << endl;
    cout << "    Completed: " << currentDuty.scheduleDate << " " << currentDuty.shiftStartTime << " - " << currentDuty.shiftEndTime << endl;
    cout << "    New Schedule: " << C_CYAN << rotatedDate << " " << rotatedStartTime << " - " << rotatedEndTime << C_RESET
         << " (" << shiftDurationHours << " hrs)" << endl;

    cout << "\n  " << C_GREEN << newDuty.vehicleID << " (" << newDuty.driverName << ")" << C_RESET << endl;
    cout << "    Status: " << C_GREEN << "Standby → On Duty" << C_RESET << endl;
    cout << "    Shift: " << C_GREEN << newDuty.scheduleDate << " " << newDuty.shiftStartTime << " - "
         << newDuty.shiftEndTime << C_RESET << " (" << calculateTimeDifference(newDuty.shiftStartTime, newDuty.shiftEndTime) << " hrs)" << endl;
}

// Dynamic rotation
void AmbulanceDispatcher::dynamicRotate()
{
    Ambulance currentDuty;
    ambulanceQueue.getFront(currentDuty);

    string currentTime = getCurrentTimeString();
    string currentDate = getCurrentDateString();

    const int MAX_AMB = 50;
    Ambulance ambulances[MAX_AMB];
    int count;
    ambulanceQueue.getAllAmbulances(ambulances, count);

    // Perform rotation
    ambulanceQueue.rotate();

    // Update new on-duty ambulance
    Ambulance newDuty = ambulances[1];
    newDuty.status = "On Duty";
    newDuty.scheduleDate = currentDate;
    newDuty.shiftStartTime = currentTime;
    newDuty.shiftEndTime = addHoursToTime(currentTime, shiftDurationHours);

    // Update the front ambulance
    ambulanceQueue.updateFront(newDuty);

    // Update all subsequent ambulances' schedules
    string nextDate = currentDate;
    string nextStart = newDuty.shiftEndTime;

    // Check if first shift crosses midnight (yes = increment date)
    if (nextStart < currentTime)
    {
        addHoursToDateTime(nextDate, currentTime, shiftDurationHours);
        nextStart = newDuty.shiftEndTime; // Keep time
    }

    // Rotate array
    Ambulance rotatedAmbulances[MAX_AMB];
    rotatedAmbulances[0] = newDuty;

    // Copy remaining ambulances and update their schedules
    for (int i = 2; i < count; i++)
    {
        rotatedAmbulances[i - 1] = ambulances[i];
        rotatedAmbulances[i - 1].scheduleDate = nextDate;
        rotatedAmbulances[i - 1].shiftStartTime = nextStart;
        rotatedAmbulances[i - 1].shiftEndTime = addHoursToTime(nextStart, shiftDurationHours);

        // Check cross midnight - increment date
        if (rotatedAmbulances[i - 1].shiftEndTime < nextStart)
        {
            addHoursToDateTime(nextDate, nextStart, shiftDurationHours);
            rotatedAmbulances[i - 1].scheduleDate = nextDate;
        }

        nextDate = rotatedAmbulances[i - 1].scheduleDate;
        nextStart = rotatedAmbulances[i - 1].shiftEndTime;
    }

    // Add the old on-duty ambulance to the end
    rotatedAmbulances[count - 1] = ambulances[0];
    rotatedAmbulances[count - 1].status = "Standby";
    rotatedAmbulances[count - 1].scheduleDate = nextDate;
    rotatedAmbulances[count - 1].shiftStartTime = nextStart;
    rotatedAmbulances[count - 1].shiftEndTime = addHoursToTime(nextStart, shiftDurationHours);

    // Handle day transition for last ambulance
    if (rotatedAmbulances[count - 1].shiftEndTime < nextStart)
    {
        addHoursToDateTime(nextDate, nextStart, shiftDurationHours);
        rotatedAmbulances[count - 1].scheduleDate = nextDate;
    }

    // Clear the queue and rebuild with new schedules
    while (!ambulanceQueue.isEmpty())
    {
        Ambulance temp;
        ambulanceQueue.dequeue(temp);
    }

    // Re-enqueue with updated schedules
    for (int i = 0; i < count; i++)
    {
        ambulanceQueue.enqueue(rotatedAmbulances[i]);
    }

    cout << "\n"
         << C_GREEN << "═══════════════════════════════════════════════" << C_RESET << endl;
    cout << C_GREEN << C_BOLD << "  ✓ DYNAMIC ROTATION COMPLETED!" << C_RESET << endl;
    cout << C_GREEN << "═══════════════════════════════════════════════" << C_RESET << endl;

    cout << "\n"
         << C_BOLD << "Shift Changes:" << C_RESET << endl;
    cout << "  " << currentDuty.vehicleID << " (" << currentDuty.driverName << ")" << endl;
    cout << "    Status: " << C_YELLOW << "On Duty → Standby" << C_RESET << endl;
    cout << "    Completed: " << currentDuty.scheduleDate << " " << currentDuty.shiftStartTime << " - " << currentDuty.shiftEndTime << endl;
    cout << "    New Schedule: " << rotatedAmbulances[count - 1].scheduleDate << " "
         << rotatedAmbulances[count - 1].shiftStartTime << " - "
         << rotatedAmbulances[count - 1].shiftEndTime << endl;

    cout << "\n  " << C_GREEN << newDuty.vehicleID << " (" << newDuty.driverName << ")" << C_RESET << endl;
    cout << "    Status: " << C_GREEN << "Standby → On Duty" << C_RESET << endl;
    cout << "    New Shift: " << C_GREEN << newDuty.scheduleDate << " " << newDuty.shiftStartTime << " - "
         << newDuty.shiftEndTime << C_RESET << " (" << shiftDurationHours << " hrs)" << endl;

    cout << C_CYAN << "\n📅 All " << count << " ambulances' schedules updated!" << C_RESET << endl;
}

// Display upcoming rotation schedule
void AmbulanceDispatcher::displayUpcomingRotation() const
{
    if (ambulanceQueue.getSize() < 2)
        return;

    cout << "\n"
         << C_CYAN << "───────────────────────────────────────────────" << C_RESET << endl;
    cout << C_BOLD << "  📅 NEXT ROTATION SCHEDULE" << C_RESET << endl;
    cout << C_CYAN << "───────────────────────────────────────────────" << C_RESET << endl;

    Ambulance current;
    ambulanceQueue.getFront(current);

    cout << "  Next rotation scheduled at: " << C_YELLOW << C_BOLD << current.shiftEndTime << C_RESET << endl;

    // Get next ambulance
    const int MAX_AMB = 50;
    Ambulance ambulances[MAX_AMB];
    int count;
    ambulanceQueue.getAllAmbulances(ambulances, count);

    if (count > 1)
    {
        cout << "  Next on duty: " << C_GREEN << ambulances[1].vehicleID
             << C_RESET << " (Driver: " << ambulances[1].driverName << ")" << endl;
        cout << "  Scheduled: " << ambulances[1].shiftStartTime << " - "
             << ambulances[1].shiftEndTime << endl;
    }

    cout << C_CYAN << "───────────────────────────────────────────────" << C_RESET << endl;

    displayAmbulanceSchedule();
}

/* ==========================  Functionality 3: Display Ambulance Schedule ========================= */
void AmbulanceDispatcher::displayAmbulanceSchedule() const
{
    if (ambulanceQueue.isEmpty())
    {
        cout << C_YELLOW << "\n⚠ INFO: " << C_RESET << "No ambulances currently registered in the system." << endl;
        return;
    }

    cout << "\n"
         << C_CYAN << string(120, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << string(40, ' ') << "AMBULANCE ROTATION SCHEDULE & DUTY ROSTER" << C_RESET << endl;
    cout << C_CYAN << string(120, '=') << C_RESET << endl;

    // Display current time
    string currentTime = getCurrentTimeString();
    cout << "Current Date: " << C_BOLD << C_CYAN << getCurrentDateString()
         << " (" << getDayOfWeek(getCurrentDateString()) << ")" << C_RESET << endl;
    cout << "Current Time: " << C_BOLD << C_CYAN << currentTime << C_RESET << endl;
    cout << "Standard Shift Duration: " << C_YELLOW << shiftDurationHours << " hours" << C_RESET << endl;
    cout << C_CYAN << string(120, '-') << C_RESET << endl;

    // Display table
    cout << left
         << setw(10) << "No."
         << setw(12) << "Vehicle ID"
         << setw(15) << "Registration"
         << setw(20) << "Driver Name"
         << setw(12) << "Status"
         << setw(16) << "Schedule Date"
         << setw(12) << "Shift Start"
         << setw(12) << "Shift End"
         << setw(10) << "Duration" << endl;
    cout << string(120, '-') << endl;

    ambulanceQueue.display();

    cout << C_CYAN << string(120, '=') << C_RESET << endl;

    // Check if rotation is needed
    if (isRotationNeeded())
    {
        cout << C_RED << C_BOLD << "\n⚠ ALERT: Current shift has ended! Rotation recommended." << C_RESET << endl;
    }
}

/* ==================================== Display duty statistics =================================*/
void AmbulanceDispatcher::displayDutyStatistics() const
{
    if (ambulanceQueue.isEmpty())
    {
        cout << C_YELLOW << "\n⚠ No ambulances registered yet." << C_RESET << endl;
        return;
    }

    const int MAX_AMB = 50;
    Ambulance ambulances[MAX_AMB];
    int count;

    ambulanceQueue.getAllAmbulances(ambulances, count);

    cout << "\n"
         << C_CYAN << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_CYAN << "           SCHEDULE & DUTY STATISTICS" << C_RESET << endl;
    cout << C_CYAN << string(70, '=') << C_RESET << endl;

    int onDutyCount = 0;
    int standbyCount = 0;
    int totalScheduledHours = 0;

    for (int i = 0; i < count; i++)
    {
        if (ambulances[i].status == "On Duty")
            onDutyCount++;
        else if (ambulances[i].status == "Standby")
            standbyCount++;

        int shiftHours = ambulances[i].getShiftDurationHours();
        totalScheduledHours += shiftHours;
    }

    // Statistic Overview
    cout << "\n"
         << C_BOLD << "Fleet Overview:" << C_RESET << endl;
    cout << "  Total Ambulances: " << C_BOLD << count << C_RESET << endl;
    cout << "  Currently " << C_GREEN << "On Duty" << C_RESET << ": " << onDutyCount << endl;
    cout << "  Currently " << C_YELLOW << "Standby" << C_RESET << ": " << standbyCount << endl;

    cout << "\n"
         << C_BOLD << "Schedule Details:" << C_RESET << endl;
    cout << "  Standard Shift: " << shiftDurationHours << " hours per ambulance" << endl;
    cout << "  Total Scheduled Hours: " << totalScheduledHours << " hours" << endl;
    cout << "  Average per Ambulance: " << (count > 0 ? totalScheduledHours / count : 0) << " hours" << endl;

    cout << "\n"
         << C_BOLD << "Rotation System:" << C_RESET << endl;
    cout << "  Type: " << C_CYAN << "Circular Queue (Linked List)" << C_RESET << endl;
    cout << "  Principle: Equal duty time distribution" << endl;
    cout << "  Coverage: Continuous 24/7 operation" << endl;
    cout << "  Rotation Modes: Normal (scheduled) & Dynamic (real-time)" << endl;

    cout << C_CYAN << string(70, '=') << C_RESET << endl;
}

/* ============================================ Set shift duration ======================================== */
void AmbulanceDispatcher::setShiftDuration(int hours)
{
    if (hours > 0 && hours <= 24)
    {
        int oldDuration = shiftDurationHours;
        shiftDurationHours = hours;

        cout << C_GREEN << "\n✓ Shift duration updated from " << oldDuration
             << " to " << hours << " hours." << C_RESET << endl;

        cout << C_YELLOW << "\nDo you want to recalculate all existing ambulance schedules? (y/n): " << C_RESET;
        char choice;
        cin >> choice;

        if (choice == 'y' || choice == 'Y')
        {
            if (!ambulanceQueue.isEmpty())
            {
                const int MAX_AMB = 50;
                Ambulance ambulances[MAX_AMB];
                int count;
                ambulanceQueue.getAllAmbulances(ambulances, count);

                // Keep 1st ambulance start time + recalculate all ambulance
                string currentDate = ambulances[0].scheduleDate;
                string currentStart = ambulances[0].shiftStartTime;

                for (int i = 0; i < count; i++)
                {
                    ambulances[i].scheduleDate = currentDate;
                    ambulances[i].shiftStartTime = currentStart;
                    ambulances[i].shiftEndTime = addHoursToTime(currentStart, hours);

                    // Handle day transition
                    bool crossesMidnight = (ambulances[i].shiftEndTime < currentStart) || (ambulances[i].shiftEndTime == "00:00");

                    if (crossesMidnight)
                    {
                        // Advance current ambulance’s schedule date
                        string tempDate = currentDate;
                        string tempTime = currentStart;
                        addHoursToDateTime(tempDate, tempTime, hours);
                        currentDate = tempDate;
                    }

                    // Update start time for next iteration
                    currentStart = ambulances[i].shiftEndTime;
                }

                // Rebuild queue
                while (!ambulanceQueue.isEmpty())
                {
                    Ambulance temp;
                    ambulanceQueue.dequeue(temp);
                }

                for (int i = 0; i < count; i++)
                {
                    ambulanceQueue.enqueue(ambulances[i]);
                }

                cout << C_GREEN << "\n✓ All " << count << " ambulance schedules updated with new duration!" << C_RESET << endl;
                cout << C_CYAN << "New schedule:" << C_RESET << endl;
                displayAmbulanceSchedule();
            }
        }
        else
        {
            cout << C_YELLOW << "Note: New duration will apply to new registrations and future rotations only." << C_RESET << endl;
        }
    }
    else
    {
        cout << C_RED << "\n✗ Invalid duration! Must be between 1-24 hours." << C_RESET << endl;
    }
}

// Display menu for ambulance dispatcher
void AmbulanceDispatcher::displayMenu()
{
    cout << "\n"
         << C_BOLD << C_BLUE << string(70, '=') << C_RESET << endl;
    cout << C_BOLD << C_BLUE << "     🚑 AMBULANCE DISPATCHER MANAGEMENT SYSTEM 🚑" << C_RESET << endl;
    cout << C_BOLD << C_BLUE << string(70, '=') << C_RESET << endl;
    cout << "  " << C_CYAN << "1." << C_RESET << " Register New Ambulance" << endl;
    cout << "  " << C_CYAN << "2." << C_RESET << " Rotate Ambulance Shift" << endl;
    cout << "  " << C_CYAN << "3." << C_RESET << " Display Ambulance Schedule" << endl;
    cout << "  " << C_CYAN << "4." << C_RESET << " View Duty Statistics" << endl;
    cout << "  " << C_CYAN << "5." << C_RESET << " Set Shift Duration (Current: " << shiftDurationHours << " hrs)" << endl;
    cout << "  " << C_RED << "6." << C_RESET << " Exit & Save" << endl;
    cout << C_BOLD << C_BLUE << string(70, '=') << C_RESET << endl;
    cout << "Enter your choice: ";
}

// Run the ambulance dispatcher system
void AmbulanceDispatcher::run()
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
            cout << C_RED << "\n✗ Invalid input! Please enter a number (1-6)." << C_RESET << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            registerAmbulance();
            break;

        case 2:
            rotateAmbulanceShift();
            break;

        case 3:
            displayAmbulanceSchedule();
            break;

        case 4:
            displayDutyStatistics();
            break;

        case 5:
        {
            int hours;
            cout << "\nEnter new shift duration (hours, 1 - 24): ";
            cin >> hours;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            setShiftDuration(hours);
            break;
        }

        case 6:
            cout << "\n"
                 << C_YELLOW << "Saving scheduling data..." << C_RESET << endl;
            cout << C_GREEN << "\n✓ Data saved successfully to ambulances.txt" << C_RESET << endl;
            cout << C_GREEN << "\n✓ Thank you for using Ambulance Dispatcher System!" << C_RESET << endl;
            running = false;
            break;

        default:
            cout << C_RED << "\n✗ Invalid choice! Please select 1-6." << C_RESET << endl;
        }

        if (running)
        {
            cout << "\n"
                 << C_YELLOW << "Press Enter to continue..." << C_RESET;
            cin.get();
        }
    }
}