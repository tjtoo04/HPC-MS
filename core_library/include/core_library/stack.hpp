#ifndef STACK_HPP
#define STACK_HPP

// #include "FileIO.hpp"
// #include "supply_item.hpp"
#include <iostream>
#include <stdexcept>

using namespace std;

template <typename T> class Stack {
private:
  static constexpr size_t MAX_BYTES = 1 * 1024 * 1024; // 1 MB
  static constexpr size_t MAX_ELEMENTS = MAX_BYTES / sizeof(T);

  T data[MAX_ELEMENTS];
  int top;

public:
  Stack() : top(-1) {}

  int getTop() const { return top; }

  void push(const T &value) {
    if (isFull())
      throw overflow_error("Stack overflow: 1MB limit reached.");
    data[++top] = value;
  }

  T pop() {
    if (isEmpty())
      throw underflow_error("Stack underflow: no elements to pop.");
    return data[top--];
  }

  T peek() const {
    if (isEmpty())
      throw underflow_error("Stack is empty.");
    return data[top];
  }

  bool isFull() const { return top >= static_cast<int>(MAX_ELEMENTS) - 1; }

  bool isEmpty() const { return top == -1; }

  size_t size() const { return top + 1; }

  void printInfo() const {
    cout << "Stack capacity (elements): " << MAX_ELEMENTS << endl;
    cout << "Approx. size in bytes: " << MAX_ELEMENTS * sizeof(T) << endl;
  }

  void displayStackItems() {
    for (int i = top; i > -1; i--) {
      this->data[i].displaySupplyItem();
    }
  }

  void loadDataIntoStack() {
    int rowCount = FileIO::getFileCount("supplyitem");
    std::string readBuffer[rowCount];

    FileIO::readFromFile("../core_library/include/data/supply_item.txt",
                         readBuffer, rowCount);

    for (string line : readBuffer) {
      int tempLimit = 4; // supply item field count
      std::string tempArr[tempLimit];
      int index = Utils::splitStringToArr(tempArr, tempLimit, line, ",");
      SupplyItem item =
          SupplyItem(tempArr[0], tempArr[1], std::stoi(tempArr[2]), tempArr[3]);

      this->push(item);
    }
  }

  void pushItemsIntoStack(T *arr, size_t size) {
    for (int i = 0; i < size; i++) {
      this->push(arr[i]);
    }
  }
};

#endif
