#include "core_library/FileIO.hpp"
#include <iostream>
int main(int argc, char *argv[]) {
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
  return 0;
}
