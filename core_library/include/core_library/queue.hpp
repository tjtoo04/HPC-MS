#include "FileIO.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Patient structure to hold patient information
struct Patient {
  std::string patientID;
  std::string name;
  std::string conditionType;

  // Default constructor
  Patient() : patientID(""), name(""), conditionType("") {}

  // Parameterized constructor
  Patient(const std::string &id, const std::string &n,
          const std::string &condition)
      : patientID(id), name(n), conditionType(condition) {}

  // Convert patient data to TXT format for file storage
  std::string toTXT() const {
    return patientID + "," + name + "," + conditionType;
  }

  // Parse TXT line to create Patient object
  static Patient fromTXT(const std::string &line) {
    Patient p;
    std::stringstream ss(line);
    std::getline(ss, p.patientID, ',');
    std::getline(ss, p.name, ',');
    std::getline(ss, p.conditionType, ',');
    return p;
  }
};

// Queue implementation for Patient Admission Management
struct Queue {
private:
  static const int MAX_SIZE = 5; // Max queue capacity
  Patient patients[MAX_SIZE];    // Array to store patients
  int frontIndex;                // Index of front element
  int rearIndex;                 // Index of rear element
  int count;                     // Current number of patients in queue
  std::string filename;          // File to persist patient data

  // Load patients from file during initialization using FileIO
  void loadFromFile() {
    std::string lines[MAX_SIZE];
    int linesRead = FileIO::readFromFile(filename, lines, MAX_SIZE);

    if (linesRead == -1) {
      // File doesn't exist yet, start with empty queue
      return;
    }

    count = 0;
    frontIndex = 0;
    rearIndex = -1;

    // Parse each line and add to queue
    for (int i = 0; i < linesRead; i++) {
      if (!lines[i].empty()) {
        rearIndex = (rearIndex + 1) % MAX_SIZE;
        patients[rearIndex] = Patient::fromTXT(lines[i]);
        count++;
      }
    }
  }

  // Save all patients to file using FileIO
  void saveToFile() {
    if (count == 0) {
      // If queue is empty, write empty file
      std::string emptyLines[1] = {""};
      FileIO::writeToFile(filename, emptyLines, 0);
      return;
    }

    std::string lines[MAX_SIZE];

    // Convert all patients in queue to TXT format
    for (int i = 0; i < count; i++) {
      int index = (frontIndex + i) % MAX_SIZE;
      lines[i] = patients[index].toTXT();
    }

    // Write to file using FileIO
    if (!FileIO::writeToFile(filename, lines, count)) {
      std::cout << "Error: Unable to save data to file.\n";
    }
  }

public:
  // Constructor - initialize queue and load existing data
  Queue(const std::string &dataFile = "queue.txt")
      : frontIndex(0), rearIndex(-1), count(0), filename(dataFile) {
    loadFromFile();
  }

  // Check if queue is empty
  bool isEmpty() const { return count == 0; }

  // Check if queue is full
  bool isFull() const { return count == MAX_SIZE; }

  // Get current number of patients
  int size() const { return count; }

  // ========== FUNCTIONALITY 1: ADMIT PATIENT ==========
  // Add a new patient to the queue
  bool admitPatient(const std::string &patientID, const std::string &name,
                    const std::string &conditionType) {
    // Validation checks
    if (isFull()) {
      std::cout << "Error: Patient queue is full. Cannot admit more "
                   "patients.\n";
      return false;
    }

    if (patientID.empty() || name.empty() || conditionType.empty()) {
      std::cout << "Error: All patient details must be provided.\n";
      return false;
    }

    // Add patient to rear of queue
    rearIndex = (rearIndex + 1) % MAX_SIZE;
    patients[rearIndex] = Patient(patientID, name, conditionType);
    count++;

    // Save to file for persistence using FileIO
    saveToFile();

    std::cout << "\n=== PATIENT ADMITTED SUCCESSFULLY ===\n";
    std::cout << "Patient ID: " << patientID << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Condition: " << conditionType << "\n";
    std::cout << "Queue Position: " << count << "\n";
    std::cout << "====================================\n\n";

    return true;
  }

  // ========== FUNCTIONALITY 2: DISCHARGE PATIENT ==========
  // Remove and return the patient at the front of queue (earliest admitted)
  bool dischargePatient() {
    if (isEmpty()) {
      std::cout << "Error: No patients in queue to discharge.\n";
      return false;
    }

    // Get patient at front of queue
    Patient discharged = patients[frontIndex];

    // Remove from queue
    frontIndex = (frontIndex + 1) % MAX_SIZE;
    count--;

    // Save updated queue to file using FileIO
    saveToFile();

    std::cout << "\n=== PATIENT DISCHARGED ===\n";
    std::cout << "Patient ID: " << discharged.patientID << "\n";
    std::cout << "Name: " << discharged.name << "\n";
    std::cout << "Condition: " << discharged.conditionType << "\n";
    std::cout << "Remaining patients in queue: " << count << "\n";
    std::cout << "==========================\n\n";

    return true;
  }

  // ========== FUNCTIONALITY 3: VIEW PATIENT QUEUE ==========
  // Display all patients currently waiting for treatment
  void viewPatientQueue() const {
    if (isEmpty()) {
      std::cout << "\n=== PATIENT QUEUE ===\n";
      std::cout << "Queue is empty. No patients waiting.\n";
      std::cout << "=====================\n\n";
      return;
    }

    std::cout << "\n=== PATIENT QUEUE ===\n";
    std::cout << "Total patients waiting: " << count << "\n";
    std::cout << "---------------------------------------------\n";
    std::cout << "Pos | Patient ID | Name | Condition\n";
    std::cout << "---------------------------------------------\n";

    // Display all patients in order (front to rear)
    for (int i = 0; i < count; i++) {
      int index = (frontIndex + i) % MAX_SIZE;
      std::cout << (i + 1) << "   | " << patients[index].patientID << " | "
                << patients[index].name << " | "
                << patients[index].conditionType << "\n";
    }

    std::cout << "---------------------------------------------\n";
    std::cout << "Next to be discharged: " << patients[frontIndex].name << "\n";
    std::cout << "=====================\n\n";
  }

  // Menu display for Patient Admission Clerk
  static void displayMenu() {
    std::cout << "\n========================================\n";
    std::cout << "   PATIENT ADMISSION CLERK SYSTEM\n";
    std::cout << "========================================\n";
    std::cout << "1. Admit Patient\n";
    std::cout << "2. Discharge Patient\n";
    std::cout << "3. View Patient Queue\n";
    std::cout << "4. Return to Main Menu\n";
    std::cout << "========================================\n";
    std::cout << "Enter your choice: ";
  }

  // Interactive menu system for the Patient Admission Clerk
  void runMenu() {
    int choice;
    std::string patientID, name, conditionType;

    do {
      displayMenu();
      std::cin >> choice;
      std::cin.ignore(); // Clear input buffer

      switch (choice) {
      case 1: // Admit Patient
        std::cout << "\n--- Admit New Patient ---\n";
        std::cout << "Enter Patient ID: ";
        std::getline(std::cin, patientID);
        std::cout << "Enter Patient Name: ";
        std::getline(std::cin, name);
        std::cout << "Enter Condition Type: ";
        std::getline(std::cin, conditionType);
        admitPatient(patientID, name, conditionType);
        break;

      case 2: // Discharge Patient
        dischargePatient();
        break;

      case 3: // View Queue
        viewPatientQueue();
        break;

      case 4: // Exit
        std::cout << "Returning to main menu...\n";
        break;

      default:
        std::cout << "Invalid choice. Please try again.\n";
      }
    } while (choice != 4);
  }
};
