#include "core_library/FileIO.hpp"
#include "core_library/ambulance.hpp"
#ifdef _WIN32
#include <windows.h>
#endif
#include "core_library/ambulance_dispatcher.hpp"
#include <iostream>

#include "core_library/stack.hpp"
#include "core_library/stack_menu.cpp"
#include "core_library/supply_item.hpp"

void ambulanceDispatchCode() {

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

void showMainMenu() {
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
      ║     Hospital Patient Care Management System  ║
      ╚══════════════════════════════════════════════╝
)" << C_RESET
         << endl;

    cout << "Options: " << endl;

    cout << "1. Patient Admission Clerk - Too Tze Jiat" << endl;
    cout << "2. Medical Supply Manager - Low Tian Wei" << endl;
    cout << "3. Emergency Department Officer - Cindy Pua Kah Qi" << endl;
    cout << "4. Ambulance Dispatcher - Kok Jia Yin" << endl;
    cout << "5. Exit" << endl;

    int choice = Utils::getInput(5, "Select role: ");
    switch (choice) {
    case 1:
      break;
    case 2:
      runStackProgram();
      break;
    case 3:
      break;
    case 4:
      ambulanceDispatchCode();
      break;
    case 5:
      cout << "Thank you for using the system!" << endl;
      isRunning = false;
      break;
    }
  }
}

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

int main(int argc, char *argv[]) {
  enableColorSupport();

  std::cout << "We Are So Cooked";

  const int MAX_LINES = 5;
  std::string data[MAX_LINES] = {
      "Alice", "Bob", "Charlie", "David", "Eve",
  };

  // Write to file
  if (FileIO::writeToFile("patients.txt", data, MAX_LINES))
    std::cout << "File written successfully!\n";
  else
    std::cout << "Failed to write file.\n";

  // Read back
  std::string readBuffer[MAX_LINES];
  int linesRead = FileIO::readFromFile("patients.txt", readBuffer, MAX_LINES);

  if (linesRead > 0) {
    std::cout << "Read " << linesRead << " lines:\n";
    for (int i = 0; i < linesRead; ++i)
      std::cout << readBuffer[i] << "\n";
  } else {
    std::cout << "Failed to read file.\n";
  }

  showMainMenu();

  return 0;
}
