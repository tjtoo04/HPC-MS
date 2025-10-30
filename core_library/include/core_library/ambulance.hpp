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
  string shiftStartTime;
  string shiftEndTime;

  // Constructor
  Ambulance() : vehicleID(""), ambulanceID(""), driverName(""),
                status("Standby"), shiftStartTime("--:--"), shiftEndTime("--:--") {}

  // Parameterized constructor
  Ambulance(string vID, string aID, string driver, string stat = "Standby",
            string startTime = "--:--", string endTime = "--:--")
      : vehicleID(vID), ambulanceID(aID), driverName(driver),
        status(stat), shiftStartTime(startTime), shiftEndTime(endTime) {}

  void display() const;

  int getShiftDurationHours() const;

  bool isShiftActive() const;

  void generateShiftTimes(string startTime, int durationHours);
};

string getCurrentTimeString();

string addHoursToTime(string timeStr, int hours);

int calculateTimeDifference(string startTime, string endTime);

#endif