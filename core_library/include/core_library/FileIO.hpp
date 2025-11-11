#pragma once
#include <fstream>
#include <string>
#include "utils.hpp"
#include <iostream>

struct FileIO
{
  static bool writeToFile(const std::string &filename, const std::string *lines,
                          int lineCount)
  {
    std::ofstream outFile(filename);
    if (!outFile.is_open())
      return false;

    for (int i = 0; i < lineCount; ++i)
    {
      outFile << lines[i];
      if (i < lineCount - 1)
        outFile << '\n';
    }

    outFile.close();
    return true;
  }

  // Read lines from a text file into a preallocated string array
  static int readFromFile(const std::string &filename, std::string *lines,
                          int maxLines)
  {
    std::ifstream inFile(filename);
    if (!inFile.is_open())
      return -1; // indicate failure

    int count = 0;
    while (count < maxLines && std::getline(inFile, lines[count]))
    {
      ++count;
    }

    inFile.close();
    return count; // number of lines read
  }

  // NOTE: SPECIFICALLY TO READ INDEX.TXT FILE ONLY CAUSE IM LAZY, DO NOT USE FOR OTHERS
  static int getFileCount(std::string targetFile)
  {
    std::string lines[3];
    std::string line;
    std::ifstream in("../core_library/include/data/index.txt");
    if (!in.is_open())
      return -1;

    while (std::getline(in, line))
    {
      int count = Utils::splitStringToArr(lines, 3, line, ",");
      if (lines[0] == targetFile)
      {
        return std::stoi(lines[1]);
      }
    }
    return -1;
  }

  // NOTE: THIS IS ALSO SPECIFICALLY FOR INDEX TEXT FILE
  static void updateFileCount(std::string targetFile, int newCount)
  {
    std::ifstream in("../core_library/include/data/index.txt");
    if (!in.is_open())
      return;

    std::ofstream temp("../core_library/include/data/index_tmp.txt");
    if (!temp.is_open())
    {
      in.close();
      return;
    }

    std::string line;
    std::string parts[3];

    while (std::getline(in, line))
    {
      int count = Utils::splitStringToArr(parts, 3, line, ",");

      if (parts[0] == targetFile)
      {
        temp << parts[0] << "," << newCount << "\n";
      }
      else
      {
        temp << line << "\n";
      }
    }

    in.close();
    temp.close();

    // Replace the original file with the updated one
    std::remove("../core_library/include/data/index.txt");
    std::rename("../core_library/include/data/index_tmp.txt", "../core_library/include/data/index.txt");
  }

  // NOTE: TO FIND A ROW FROM VALUE X OF INDEX Y
  static std::string readLineFromFile(const std::string &filename, std::string targetValue, int targetIndex = 0)
  {
    const int arrUpperLimit = 100;
    std::string lines[arrUpperLimit];
    std::string line;

    std::ifstream in(filename);
    if (!in.is_open())
      return "";

    while (std::getline(in, line))
    {
      int count = Utils::splitStringToArr(lines, arrUpperLimit, line, ",");
      if (targetIndex < count && lines[targetIndex] == targetValue)
      {
        return line;
      }
    }

    return "";
  }

  // NOTE: YOUR CLASS REQUIRES A toString() METHOD FOR IT TO WORK
  template <typename T>
  static void appendObjectsToFile(const std::string &filename, T *objectArr, int count)
  {
    std::ofstream outFile(filename, std::ios::app);
    if (!outFile.is_open())
      return;

    for (int i = 0; i < count; ++i)
    {
      outFile << objectArr[i].toString() << "\n";
    }

    outFile.close();
  }

  static void removeLastLineFromFile(const std::string &filename)
  {
    std::ifstream inFile(filename);
    if (!inFile.is_open())
    {
      std::cerr << "Error: Could not open file for reading: " << filename << std::endl;
      return;
    }

    std::ofstream tempFile("temp_file.tmp");
    if (!tempFile.is_open())
    {
      std::cerr << "Error: Could not create temporary file.\n";
      return;
    }

    std::string line;
    std::string prevLine;
    bool firstLine = true;

    while (std::getline(inFile, line))
    {
      if (!firstLine)
      {
        tempFile << prevLine << '\n';
      }
      prevLine = line;
      firstLine = false;
    }

    inFile.close();
    tempFile.close();

    if (std::remove(filename.c_str()) != 0)
    {
      std::cerr << "Error: Could not remove original file.\n";
      return;
    }
    if (std::rename("temp_file.tmp", filename.c_str()) != 0)
    {
      std::cerr << "Error: Could not rename temp file.\n";
      return;
    }
  }
};
