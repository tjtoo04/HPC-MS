#ifndef STACK_HPP
#define STACK_HPP

#include "FileIO.hpp"
#include "supply_item.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T>
class Stack
{
private:
  static constexpr size_t MAX_BYTES = 1 * 1024 * 1024; // 1 MB
  static constexpr size_t MAX_ELEMENTS = MAX_BYTES / sizeof(T);

  T *data;
  int top;

public:
  Stack() : data(new T[MAX_ELEMENTS]), top(-1) {}

  ~Stack() { delete[] data; }

  Stack(const Stack &other) : data(new T[MAX_ELEMENTS]), top(other.top)
  {
    for (int i = 0; i <= top; i++)
    {
      data[i] = other.data[i];
    }
  }

  Stack &operator=(const Stack &other)
  {
    if (this != &other)
    {
      delete[] data;
      data = new T[MAX_ELEMENTS];
      top = other.top;
      for (int i = 0; i <= top; i++)
      {
        data[i] = other.data[i];
      }
    }
    return *this;
  }

  void push(const T &value)
  {
    if (isFull())
      throw overflow_error("Stack overflow: 1MB limit reached.");
    data[++top] = value;
  }

  T pop()
  {
    if (isEmpty())
      throw underflow_error("Stack underflow: no elements to pop.");
    return data[top--];
  }

  T peek() const
  {
    if (isEmpty())
      throw underflow_error("Stack is empty.");
    return data[top];
  }

  bool isFull() const { return top >= static_cast<int>(MAX_ELEMENTS) - 1; }

  bool isEmpty() const { return top == -1; }

  size_t size() const { return top + 1; }

  void printInfo() const
  {
    cout << "Stack capacity (elements): " << MAX_ELEMENTS << endl;
    cout << "Approx. size in bytes: " << MAX_ELEMENTS * sizeof(T) << endl;
  }

  void displayStackItems()
  {
    for (int i = top; i > -1; i--)
    {
      this->data[i].displaySupplyItem();
    }
  }

  void loadDataIntoStack()
  {
    int rowCount = FileIO::getFileCount("supplyitem");
    if (rowCount <= 0)
      return;

    std::string *readBuffer = new std::string[rowCount];
    FileIO::readFromFile("../core_library/include/data/supply_item.txt", readBuffer, rowCount);

    for (int i = 0; i < rowCount; ++i)
    {
      const std::string &line = readBuffer[i];

      const int tempLimit = 4;
      std::string tempArr[tempLimit];

      int fieldCount = Utils::splitStringToArr(tempArr, tempLimit, line, ",");
      if (fieldCount == tempLimit)
      {
        int qty = std::stoi(tempArr[2]);
        SupplyItem item(tempArr[0], tempArr[1], qty, tempArr[3]);
        this->push(item);
      }
    }

    delete[] readBuffer;
  }

  void pushItemsIntoStack(T *arr, size_t size)
  {
    for (int i = 0; i < size; i++)
    {
      this->push(arr[i]);
    }
  }
};

#endif