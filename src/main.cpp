#include "core_library/FileIO.hpp"
#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include "core_library/ambulance_dispatcher.hpp"

void enableColorSupport()
{
#ifdef _WIN32
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD mode;
  GetConsoleMode(hConsole, &mode);
  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hConsole, mode);
  SetConsoleOutputCP(CP_UTF8);
#endif
}

int main(int argc, char *argv[])
{
  enableColorSupport();
  
  std::cout << "We Are So Cooked";

  const int MAX_LINES = 5;
  std::string data[MAX_LINES] = {
      "Alice",
      "Bob",
      "Charlie",
      "David",
      "Eve",
  };

  // Write to file
  if (FileIO::writeToFile("patients.txt", data, MAX_LINES))
    std::cout << "File written successfully!\n";
  else
    std::cout << "Failed to write file.\n";

  // Read back
  std::string readBuffer[MAX_LINES];
  int linesRead = FileIO::readFromFile("patients.txt", readBuffer, MAX_LINES);

  if (linesRead > 0)
  {
    std::cout << "Read " << linesRead << " lines:\n";
    for (int i = 0; i < linesRead; ++i)
      std::cout << readBuffer[i] << "\n";
  }
  else
  {
    std::cout << "Failed to read file.\n";
  }

  /* Role 4: Ambulance Dispatcher */

  cout << "\n"
       << C_BOLD << C_GREEN << "╔═══════════════════════════════════════════════╗" << C_RESET << endl;
  cout << C_BOLD << C_GREEN << "║   ROLE 4: Ambulance Dispatcher Management     ║" << C_RESET << endl;
  cout << C_BOLD << C_GREEN << "║           (KOK JIA YIN - TP071062)            ║" << C_RESET << endl;
  cout << C_BOLD << C_GREEN << "╚═══════════════════════════════════════════════╝" << C_RESET << endl;

  AmbulanceDispatcher dispatcher;
  dispatcher.run();

  return 0;
}
