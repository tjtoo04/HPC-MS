#ifndef AMBULANCE_HPP
#define AMBULANCE_HPP

#include <string>
#include <ctime>
using namespace std;

struct Ambulance
{
  string vehicleID;
  string ambulanceID;
  string driverName;
  string status;
  string scheduleDate;
  string shiftStartTime;
  string shiftEndTime;

  // Constructor
  Ambulance() : vehicleID(""), ambulanceID(""), driverName(""),
                status("Standby"), scheduleDate("--"), shiftStartTime("--:--"), shiftEndTime("--:--") {}

  // Parameterized constructor
  Ambulance(string vID, string aID, string driver, string stat = "Standby", string date = "--",
            string startTime = "--:--", string endTime = "--:--")
      : vehicleID(vID), ambulanceID(aID), driverName(driver),
        status(stat), scheduleDate(date), shiftStartTime(startTime), shiftEndTime(endTime) {}

  void display() const;

  int getShiftDurationHours() const;

  bool isShiftActive() const;

  void generateShiftTimes(string startTime, int durationHours);
};

string getCurrentTimeString();

string getCurrentDateString();

string getDayOfWeek();

string addHoursToTime(string timeStr, int hours);

void addHoursToDateTime(string& date, string& time, int hours);

int calculateTimeDifference(string startTime, string endTime);

#endif