#include <string>
#include <iostream>

struct Utils
{
  static int splitStringToArr(std::string *arr, int arrUpperLimit, const std::string &s, const std::string &delimiter)
  {
    int arrPos = 0;
    size_t start = 0;
    size_t pos = 0;

    while ((pos = s.find(delimiter, start)) != std::string::npos && arrPos < arrUpperLimit)
    {
      arr[arrPos++] = s.substr(start, pos - start);
      start = pos + delimiter.length();
    }
    if (arrPos < arrUpperLimit)
      arr[arrPos++] = s.substr(start);

    return arrPos;
  }

  // Clear leftover newline in input buffer
  static void clearInputBuffer()
  {
    if (std::cin.peek() == '\n')
      std::cin.ignore();
  }

  // Integer input with validation
  static int getIntInput(const std::string &prompt, int min, int max)
  {
    int choice;
    while (true)
    {
      std::cout << prompt;
      if (std::cin >> choice)
      {
        if (choice >= min && choice <= max)
        {
          clearInputBuffer();
          return choice;
        }
        std::cout << "Invalid choice. Please enter between "
                  << min << " and " << max << ".\n";
      }
      else
      {
        std::cout << "Invalid input. Please enter a number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
};
