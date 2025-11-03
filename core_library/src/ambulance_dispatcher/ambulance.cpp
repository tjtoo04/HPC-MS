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
         << setw(16) << scheduleDate
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

// Get current date in YYYY-MM-DD format
string getCurrentDateString()
{
    time_t now = time(nullptr);
    tm *localTime = localtime(&now);

    stringstream ss;
    ss << (localTime->tm_year + 1900) << "-"
       << setfill('0') << setw(2) << (localTime->tm_mon + 1) << "-"
       << setfill('0') << setw(2) << localTime->tm_mday;

    return ss.str();
}

// Get day of week (Mon, Tue, Wed, etc.)
string getDayOfWeek(string dateStr)
{
    time_t now = time(nullptr);
    tm *localTime = localtime(&now);

    const char *days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    return days[localTime->tm_wday];
}

// Add hours to datetime (handles day transition)
void addHoursToDateTime(string &date, string &time, int hours)
{
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));

    hour += hours;

    // Calculate days to add
    int daysToAdd = hour / 24;
    hour = hour % 24;

    stringstream ss;
    ss << setfill('0') << setw(2) << hour << ":"
       << setfill('0') << setw(2) << minute;
    time = ss.str();

    if (daysToAdd > 0)
    {
        // Parse date (YYYY-MM-DD)
        int year = stoi(date.substr(0, 4));
        int month = stoi(date.substr(5, 2));
        int day = stoi(date.substr(8, 2));

        day += daysToAdd;

        int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
        while (day > daysInMonth[month - 1])
        {
            day -= daysInMonth[month - 1];
            month++;
            if (month > 12)
            {
                month = 1;
                year++;
            }
        }

        stringstream dateStream;
        dateStream << year << "-"
                   << setfill('0') << setw(2) << month << "-"
                   << setfill('0') << setw(2) << day;
        date = dateStream.str();
    }
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