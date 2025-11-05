#include <string>

struct Utils {
  static int splitStringToArr(std::string* arr, int arrUpperLimit, const std::string& s, const std::string& delimiter) {
    int arrPos = 0;
    size_t start = 0;
    size_t pos = 0;

    while ((pos = s.find(delimiter, start)) != std::string::npos && arrPos < arrUpperLimit) {
      arr[arrPos++] = s.substr(start, pos - start);
      start = pos + delimiter.length();
    }
    if (arrPos < arrUpperLimit)
      arr[arrPos++] = s.substr(start);

    return arrPos;
  }
};
