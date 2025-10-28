#pragma once
#include <fstream>
#include <string>

struct FileIO {
  static bool writeToFile(const std::string &filename, const std::string *lines,
                          int lineCount) {
    std::ofstream outFile(filename);
    if (!outFile.is_open())
      return false;

    for (int i = 0; i < lineCount; ++i) {
      outFile << lines[i];
      if (i < lineCount - 1)
        outFile << '\n';
    }

    outFile.close();
    return true;
  }

  // Read lines from a text file into a preallocated string array
  static int readFromFile(const std::string &filename, std::string *lines,
                          int maxLines) {
    std::ifstream inFile(filename);
    if (!inFile.is_open())
      return -1; // indicate failure

    int count = 0;
    while (count < maxLines && std::getline(inFile, lines[count])) {
      ++count;
    }

    inFile.close();
    return count; // number of lines read
  }
};
