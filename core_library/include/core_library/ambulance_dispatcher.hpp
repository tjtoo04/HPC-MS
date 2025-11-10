#ifndef AMBULANCEDISPATCHER_HPP
#define AMBULANCEDISPATCHER_HPP

#include "circular_queue.hpp"
#include "ambulance.hpp"
#include <string>
using namespace std;

// ANSI color
#define C_RESET "\033[0m"
#define C_RED "\033[31m"
#define C_GREEN "\033[32m"
#define C_YELLOW "\033[33m"
#define C_BLUE "\033[34m"
#define C_MAGENTA "\033[35m"
#define C_CYAN "\033[36m"
#define C_BOLD "\033[1m"

class AmbulanceDispatcher
{
private:
    CircularQueue ambulanceQueue;
    int nextAmbulanceNumber;      // Auto-increment counter for ambulance IDs
    int shiftDurationHours;       // Standard shift duration (default: 8 hours)

    string generateNextAmbulanceID();

    bool isRotationNeeded() const;

    string formatDuration(int hours) const;

public:
    AmbulanceDispatcher();

    ~AmbulanceDispatcher();

    // Functionality 1: Register Ambulance
    void registerAmbulance();

    // Functionality 2: Rotate Ambulance Shift
    void rotateAmbulanceShift();

    void normalRotate();

    void dynamicRotate();

    void displayUpcomingRotation() const;

    // Functionality 3: Display Ambulance Schedule
    void displayAmbulanceSchedule() const;

    void autoScheduleCheck();

    void displayDutyStatistics() const;

    void setShiftDuration(int hours);

    // Load ambulances from file (helper function)
    void loadAmbulancesFromFile(const string &filename);

    // Save ambulances to file (helper function)
    void saveAmbulancesToFile(const string &filename) const;

    void displayMenu();

    void run();

};

#endif