#include <iostream>
#include <string>

struct Utils {
  static int splitStringToArr(std::string *arr, int arrUpperLimit,
                              const std::string &s,
                              const std::string &delimiter) {
    int arrPos = 0;
    size_t start = 0;
    size_t pos = 0;

    while ((pos = s.find(delimiter, start)) != std::string::npos &&
           arrPos < arrUpperLimit) {
      arr[arrPos++] = s.substr(start, pos - start);
      start = pos + delimiter.length();
    }
    if (arrPos < arrUpperLimit)
      arr[arrPos++] = s.substr(start);

    return arrPos;
  }

  static bool isInteger(const std::string &s) {
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
      return false;

    char *p;
    strtol(s.c_str(), &p, 10); // Attempt to convert to long

    return (*p == 0); // Check if the entire string was consumed
  }

  static int getInput(int maxRange, std::string prompt = "Enter your input: ") {
    std::string input;
    std::cout << prompt << '\n';
    std::cin >> input;
    if (!isInteger(input)) {
      std::cout << "Input must be an integer!" << '\n';
      return -1;
    } else {
      if (std::stoi(input) > maxRange) {
        std::cout << "Input is too large. Try again" << '\n';
      }

      return std::stoi(input);
    }
  }
};
