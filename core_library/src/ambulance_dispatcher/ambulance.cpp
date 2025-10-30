#include "core_library/ambulance.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
using namespace std;

void Ambulance::display() const
{
    // Color coding based on status
    string statusColor;
    if (status == "On Duty")
        statusColor = "\033[32m"; // Green - On Duty
    else if (status == "Standby")
        statusColor = "\033[33m"; // Yellow - Standby
    else
        statusColor = "\033[31m"; // Red - Off Duty

    cout << left << setw(12) << vehicleID
         << setw(15) << ambulanceID
         << setw(20) << driverName
         << statusColor << setw(12) << status << "\033[0m"
         << setw(12) << shiftStartTime
         << setw(12) << shiftEndTime;

    int duration = getShiftDurationHours();
    cout << setw(10) << (duration > 0 ? to_string(duration) + " hrs" : "-") << endl;
}

// Calculate shift duration in hours
int Ambulance::getShiftDurationHours() const
{
    if (shiftStartTime == "--:--" || shiftEndTime == "--:--")
    {
        return 0;
    }
    return calculateTimeDifference(shiftStartTime, shiftEndTime);
}

// Active shift check
bool Ambulance::isShiftActive() const
{
    if (shiftStartTime == "--:--" || shiftEndTime == "--:--")
    {
        return false;
    }

    string currentTime = getCurrentTimeString();

    // Same day comparison
    return (currentTime >= shiftStartTime && currentTime < shiftEndTime);
}

// Generate shift times based on start time and duration
void Ambulance::generateShiftTimes(string startTime, int durationHours)
{
    shiftStartTime = startTime;
    shiftEndTime = addHoursToTime(startTime, durationHours);
}

// Get current time in HH:MM format
string getCurrentTimeString()
{
    time_t now = time(nullptr);
    tm *localTime = localtime(&now);

    stringstream ss;
    ss << setfill('0') << setw(2) << localTime->tm_hour << ":"
       << setfill('0') << setw(2) << localTime->tm_min;

    return ss.str();
}

// Add hours to time string (HH:MM format)
string addHoursToTime(string timeStr, int hours)
{
    int hour = stoi(timeStr.substr(0, 2));
    int minute = stoi(timeStr.substr(3, 2));

    hour = (hour + hours) % 24; // hour

    stringstream ss;
    ss << setfill('0') << setw(2) << hour << ":"
       << setfill('0') << setw(2) << minute;

    return ss.str();
}

// Calculate time difference in hours
int calculateTimeDifference(string startTime, string endTime)
{
    int startHour = stoi(startTime.substr(0, 2));
    int startMin = stoi(startTime.substr(3, 2));

    int endHour = stoi(endTime.substr(0, 2));
    int endMin = stoi(endTime.substr(3, 2));

    // Convert to minutes
    int startTotalMin = startHour * 60 + startMin;
    int endTotalMin = endHour * 60 + endMin;

    // Handle next day scenario
    if (endTotalMin < startTotalMin)
    {
        endTotalMin += 24 * 60;
    }

    // Calculate difference in hours
    int diffMin = endTotalMin - startTotalMin;
    return diffMin / 60;
}