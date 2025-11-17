#include "core_library/FileIO.hpp"
#include "core_library/queue.hpp"
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

#include "core_library/ambulance_dispatcher.hpp"
#include "core_library/emergency_department/emergency_officer.hpp"

#include "core_library/stack.hpp"
#include "core_library/stack_menu.cpp"
#include "core_library/supply_item.hpp"
#include <iostream>

void enableColorSupport() {
#ifdef _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode;
  GetConsoleMode(hConsole, &mode);
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hConsole, mode);
  SetConsoleOutputCP(CP_UTF8);
#endif
}

void runQueue() {
  Queue patientQueue("queue.txt");

  std::cout << "===========================================\n";
  std::cout << "  HOSPITAL PATIENT CARE MANAGEMENT SYSTEM\n";
  std::cout << "===========================================\n\n";

  patientQueue.runMenu();
}

void ambulanceDispatch() {

  /* Role 4: Ambulance Dispatcher */

  cout << C_BOLD << C_CYAN << R"(
      ██████╗██╗██████╗  ██████╗██╗   ██╗██╗      █████╗ ██████╗     
    ██╔════╝██║██╔══██╗██╔════╝██║   ██║██║     ██╔══██╗██╔══██╗    
    ██║     ██║██████╔╝██║     ██║   ██║██║     ███████║██████╔╝    
    ██║     ██║██╔══██╗██║     ██║   ██║██║     ██╔══██║██╔══██╗    
    ╚██████╗██║██║  ██║╚██████╗╚██████╔╝███████╗██║  ██║██║  ██║    
    ╚═════╝╚═╝╚═╝  ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝    
                                                                    
            ██████╗ ██╗   ██╗███████╗██╗   ██╗███████╗             
            ██╔═══██╗██║   ██║██╔════╝██║   ██║██╔════╝             
            ██║   ██║██║   ██║█████╗  ██║   ██║█████╗               
            ██║▄▄ ██║██║   ██║██╔══╝  ██║   ██║██╔══╝               
            ╚██████╔╝╚██████╔╝███████╗╚██████╔╝███████╗             
            ╚══▀▀═╝  ╚═════╝ ╚══════╝ ╚═════╝ ╚══════╝             
    )" << C_RESET
       << endl;

  cout << "\n"
       << C_BOLD << C_GREEN
       << "        ╔═══════════════════════════════════════════════╗" << C_RESET
       << endl;
  cout << C_BOLD << C_GREEN
       << "        ║   ROLE 4: Ambulance Dispatcher Management     ║" << C_RESET
       << endl;
  cout << C_BOLD << C_GREEN
       << "        ║           (KOK JIA YIN - TP071062)            ║" << C_RESET
       << endl;
  cout << C_BOLD << C_GREEN
       << "        ╚═══════════════════════════════════════════════╝" << C_RESET
       << endl;

  AmbulanceDispatcher dispatcher;
  dispatcher.run();
}

void priorityQueueSystem()
{
  cout << C_BOLD << C_CYAN << R"(
    ██████╗ ██████╗ ██╗ ██████╗ ██████╗ ██╗████████╗██╗   ██╗
    ██╔══██╗██╔══██╗██║██╔═══██╗██╔══██╗██║╚══██╔══╝╚██╗ ██╔╝
    ██████╔╝██████╔╝██║██║   ██║██████╔╝██║   ██║    ╚████╔╝ 
    ██╔═══╝ ██╔══██╗██║██║   ██║██╔══██╗██║   ██║     ╚██╔╝  
    ██║     ██║  ██║██║╚██████╔╝██║  ██║██║   ██║      ██║   
    ╚═╝     ╚═╝  ╚═╝╚═╝ ╚═════╝ ╚═╝  ╚═╝╚═╝   ╚═╝      ╚═╝   
                                                              
            ██████╗ ██╗   ██╗███████╗██╗   ██╗███████╗      
            ██╔═══██╗██║   ██║██╔════╝██║   ██║██╔════╝      
            ██║   ██║██║   ██║█████╗  ██║   ██║█████╗        
            ██║▄▄ ██║██║   ██║██╔══╝  ██║   ██║██╔══╝        
            ╚██████╔╝╚██████╔╝███████╗╚██████╔╝███████╗      
            ╚══▀▀═╝  ╚═════╝ ╚══════╝ ╚═════╝ ╚══════╝      
    )" << C_RESET
       << endl;

  cout << "\n"
       << C_BOLD << C_GREEN
       << "        ╔═══════════════════════════════════════════════╗" << C_RESET << endl;
  cout << C_BOLD << C_GREEN
       << "        ║   ROLE 3: Emergency Department Officer        ║" << C_RESET << endl;
  cout << C_BOLD << C_GREEN
       << "        ║       (CINDY PUA KAH QI - TP071189)           ║" << C_RESET << endl;
  cout << C_BOLD << C_GREEN
       << "        ║                                               ║" << C_RESET << endl;
  cout << C_BOLD << C_CYAN
       << "        ║   Data Structure: MIN-HEAP PRIORITY QUEUE     ║" << C_RESET << endl;
  cout << C_BOLD << C_CYAN
       << "        ║   Time Complexity: O(log n) operations        ║" << C_RESET << endl;
  cout << C_BOLD << C_GREEN
       << "        ╚═══════════════════════════════════════════════╝" << C_RESET << endl;

  EmergencyOfficer officer;
  officer.run();
}

void showMainMenu()
{
  bool isRunning = true;
  while (isRunning) {
    cout << C_BOLD << C_CYAN << R"(
                  ██╗  ██╗██████╗  ██████╗    ███╗   ███╗███████╗
                  ██║  ██║██╔══██╗██╔════╝    ████╗ ████║██╔════╝
                  ███████║██████╔╝██║         ██╔████╔██║███████╗
                  ██╔══██║██╔═══╝ ██║         ██║╚██╔╝██║╚════██║
                  ██║  ██║██║     ╚██████╗    ██║ ╚═╝ ██║███████║
                  ╚═╝  ╚═╝╚═╝      ╚═════╝    ╚═╝     ╚═╝╚══════╝
                                                          
)" << C_RESET
         << endl;

    cout << C_BOLD << C_GREEN << R"(
                  ╔══════════════════════════════════════════════╗
                  ║   Hospital Patient Care Management System    ║
                  ║                 (Group 15)                   ║
                  ╚══════════════════════════════════════════════╝
)" << C_RESET
         << endl;

    cout << C_BOLD << "                                 ▼ Select a Role ▼\n"
         << C_RESET;

    cout << C_YELLOW << R"(
            ┌─────────────────────────────────────────────────────────────┐
            │  1. Patient Admission Clerk         - Too Tze Jiat          │
            │  2. Medical Supply Manager          - Low Tian Wei          │
            │  3. Emergency Department Officer    - Cindy Pua Kah Qi      │
            │  4. Ambulance Dispatcher            - Kok Jia Yin           │
            │  5. Exit                                                    │
            └─────────────────────────────────────────────────────────────┘
    )" << C_RESET;

    int choice = Utils::getIntInput("\nSelect the role number: ", 1, 5);

    switch (choice) {
    case 1:
      runQueue();
      // Patient Admission Clerk
      break;
    case 2:
      // Medical Supply Manager
      runStackProgram();
      break;
    case 3:
      // Emergency Department Officer 
      priorityQueueSystem();
      break;
    case 4:
      // Ambulance Dispatcher
      ambulanceDispatch();
      break;
    case 5:
      cout << C_BOLD << C_CYAN << R"(
                  ╔══════════════════════════════════════════════╗
                  ║        Thank you for using the system!       ║
                  ╚══════════════════════════════════════════════╝
)" << C_RESET
           << endl;
      isRunning = false;
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  enableColorSupport();

  showMainMenu();

  return 0;
}
